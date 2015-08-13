#include <ctype.h>
#include "dirent.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "ignore.h"
#include "log.h"
#include "options.h"
#include "scandir.h"
#include "util.h"

#ifdef _WIN32
#include <shlwapi.h>
#define fnmatch(x, y, z) (!PathMatchSpecW(y, x))
#else
#include <fnmatch.h>
const int fnmatch_flags = FNM_PATHNAME;
#endif

ignores *root_ignores;

/* TODO: build a huge-ass list of files we want to ignore by default (build cache stuff, pyc files, etc) */

const wchar_t *evil_hardcoded_ignore_files[] = {
    L".",
    L"..",
    NULL
};

/* Warning: changing the first string will break skip_vcs_ignores. */
const wchar_t *ignore_pattern_files[] = {
    L".agignore",
    L".gitignore",
    L".git/info/exclude",
    L".hgignore",
    L".svn",
    NULL
};

int is_empty(ignores *ig) {
    return (ig->extensions_len + ig->names_len + ig->slash_names_len + ig->regexes_len + ig->slash_regexes_len == 0);
};

ignores *init_ignore(ignores *parent, const wchar_t *dirname, const size_t dirname_len) {
    ignores *ig = (ignores *)ag_malloc(sizeof(ignores));
    ig->extensions = NULL;
    ig->extensions_len = 0;
    ig->names = NULL;
    ig->names_len = 0;
    ig->slash_names = NULL;
    ig->slash_names_len = 0;
    ig->regexes = NULL;
    ig->regexes_len = 0;
    ig->slash_regexes = NULL;
    ig->slash_regexes_len = 0;
    ig->dirname = dirname;
    ig->dirname_len = dirname_len;

    if (parent && is_empty(parent) && parent->parent) {
        ig->parent = parent->parent;
    } else {
        ig->parent = parent;
    }

    if (parent && parent->abs_path_len > 0) {
        ag_asprintf(&(ig->abs_path), L"%s/%s", parent->abs_path, dirname);
        ig->abs_path_len = parent->abs_path_len + 1 + dirname_len;
    } else if (dirname_len == 1 && dirname[0] == '.') {
		ig->abs_path = (wchar_t *)ag_malloc(sizeof(wchar_t));
        ig->abs_path[0] = '\0';
        ig->abs_path_len = 0;
    } else {
        ag_asprintf(&(ig->abs_path), L"%s", dirname);
        ig->abs_path_len = dirname_len;
    }
    return ig;
}

void cleanup_ignore(ignores *ig) {
    if (ig == NULL) {
        return;
    }
    free_strings(ig->extensions, ig->extensions_len);
    free_strings(ig->names, ig->names_len);
    free_strings(ig->slash_names, ig->slash_names_len);
    free_strings(ig->regexes, ig->regexes_len);
    free_strings(ig->slash_regexes, ig->slash_regexes_len);
    if (ig->abs_path) {
        free(ig->abs_path);
    }
    free(ig);
}

void add_ignore_pattern(ignores *ig, const wchar_t *pattern) {
    int i;
    int pattern_len;

    /* Strip off the leading dot so that matches are more likely. */
    if (wcsncmp(pattern, L"./", 2) == 0) {
        pattern++;
    }

    /* Kill trailing whitespace */
    for (pattern_len = wcslen(pattern); pattern_len > 0; pattern_len--) {
        if (!isspace(pattern[pattern_len - 1])) {
            break;
        }
    }

    if (pattern_len == 0) {
        log_debug(L"Pattern is empty. Not adding any ignores.");
        return;
    }

	wchar_t ***patterns_p;
    size_t *patterns_len;
    if (is_fnmatch(pattern)) {
        if (pattern[0] == '*' && pattern[1] == '.' && !(is_fnmatch(pattern + 2))) {
            patterns_p = &(ig->extensions);
            patterns_len = &(ig->extensions_len);
            pattern += 2;
        } else if (pattern[0] == '/') {
            patterns_p = &(ig->slash_regexes);
            patterns_len = &(ig->slash_regexes_len);
            pattern++;
            pattern_len--;
        } else {
            patterns_p = &(ig->regexes);
            patterns_len = &(ig->regexes_len);
        }
    } else {
        if (pattern[0] == '/') {
            patterns_p = &(ig->slash_names);
            patterns_len = &(ig->slash_names_len);
            pattern++;
            pattern_len--;
        } else {
            patterns_p = &(ig->names);
            patterns_len = &(ig->names_len);
        }
    }

    ++*patterns_len;

	wchar_t **patterns;

    /* a balanced binary tree is best for performance, but I'm lazy */
	*patterns_p = patterns = (wchar_t **)ag_realloc(*patterns_p, (*patterns_len) * sizeof(wchar_t *));
    /* TODO: de-dupe these patterns */
    for (i = *patterns_len - 1; i > 0; i--) {
        if (wcscmp(pattern, patterns[i - 1]) > 0) {
            break;
        }
        patterns[i] = patterns[i - 1];
    }
    patterns[i] = ag_strndup(pattern, pattern_len);
    log_debug(L"added ignore pattern %s to %s", pattern,
              ig == root_ignores ? L"root ignores" : ig->abs_path);
}

/* For loading git/hg ignore patterns */
void load_ignore_patterns(ignores *ig, const wchar_t *path) {
    FILE *fp = NULL;
    fp = _wfopen(path, L"r");
    if (fp == NULL) {
        log_debug(L"Skipping ignore file %s: not readable", path);
        return;
    }
    log_debug(L"Loading ignore file %s.", path);

	wchar_t *line = NULL;
    ssize_t line_len = 0;
    size_t line_cap = 0;

    while ((line_len = getline(&line, &line_cap, fp)) > 0) {
        if (line_len == 0 || line[0] == '\n' || line[0] == '#') {
            continue;
        }
        if (line[line_len - 1] == '\n') {
            line[line_len - 1] = '\0'; /* kill the \n */
        }
        add_ignore_pattern(ig, line);
    }

    free(line);
    fclose(fp);
}

void load_svn_ignore_patterns(ignores *ig, const wchar_t *path) {
    FILE *fp = NULL;
	wchar_t *dir_prop_base;
    ag_asprintf(&dir_prop_base, L"%s/%s", path, SVN_DIR_PROP_BASE);

    fp = _wfopen(dir_prop_base, L"r");
    if (fp == NULL) {
        log_debug(L"Skipping svn ignore file %s", dir_prop_base);
        free(dir_prop_base);
        return;
    }

	wchar_t *entry = NULL;
    size_t entry_len = 0;
	wchar_t *key = (wchar_t*)ag_malloc(32); /* Sane start for max key length. */
    size_t key_len = 0;
    size_t bytes_read = 0;
	wchar_t *entry_line;
    size_t line_len;
    int matches;

    while (fwscanf(fp, L"K %zu\n", &key_len) == 1) {
		key = (wchar_t*)ag_realloc(key, key_len + 1);
        bytes_read = fread(key, 1, key_len, fp);
        key[key_len] = '\0';
        matches = fwscanf(fp, L"\nV %zu\n", &entry_len);
        if (matches != 1) {
            log_debug(L"Unable to parse svnignore file %s: fscanf() got %i matches, expected 1.", dir_prop_base, matches);
            goto cleanup;
        }

        if (wcsncmp(SVN_PROP_IGNORE, key, bytes_read) != 0) {
            log_debug(L"key is %s, not %s. skipping %u bytes", key, SVN_PROP_IGNORE, entry_len);
            /* Not the key we care about. fseek and repeat */
            fseek(fp, entry_len + 1, SEEK_CUR); /* +1 to account for newline. yes I know this is hacky */
            continue;
        }
        /* Aww yeah. Time to ignore stuff */
		entry = (wchar_t*)ag_malloc(entry_len + 1);
        bytes_read = fread(entry, 1, entry_len, fp);
        entry[bytes_read] = '\0';
        log_debug(L"entry: %s", entry);
        break;
    }
    if (entry == NULL) {
        goto cleanup;
    }
	wchar_t *patterns = entry;
    size_t patterns_len = wcslen(patterns);
    while (*patterns != '\0' && patterns < (entry + bytes_read)) {
        for (line_len = 0; line_len < patterns_len; line_len++) {
            if (patterns[line_len] == '\n') {
                break;
            }
        }
        if (line_len > 0) {
            entry_line = ag_strndup(patterns, line_len);
            add_ignore_pattern(ig, entry_line);
            free(entry_line);
        }
        patterns += line_len + 1;
        patterns_len -= line_len + 1;
    }
    free(entry);
cleanup:
    free(dir_prop_base);
    free(key);
    fclose(fp);
}

static int ackmate_dir_match(const wchar_t *dir_name) {
    if (opts.ackmate_dir_filter == NULL) {
        return 0;
    }
    /* we just care about the match, not where the matches are */
    return pcre16_exec(opts.ackmate_dir_filter, NULL, (PCRE_SPTR16)dir_name, wcslen(dir_name), 0, 0, NULL, 0);
}

/* This is the hottest code in Ag. 10-15% of all execution time is spent here */
static int path_ignore_search(const ignores *ig, const wchar_t *path, const wchar_t *filename) {
    wchar_t *temp;
    size_t i;
    int match_pos;

    match_pos = binary_search(filename, ig->names, 0, ig->names_len);
    if (match_pos >= 0) {
        log_debug(L"file %s ignored because name matches static pattern %s", filename, ig->names[match_pos]);
        return 1;
    }

    ag_asprintf(&temp, L"%s/%s", path[0] == '.' ? path + 1 : path, filename);

    if (wcsncmp(temp, ig->abs_path, ig->abs_path_len) == 0) {
		wchar_t *slash_filename = temp + ig->abs_path_len;
        if (slash_filename[0] == '/') {
            slash_filename++;
        }
        match_pos = binary_search(slash_filename, ig->names, 0, ig->names_len);
        if (match_pos >= 0) {
            log_debug(L"file %s ignored because name matches static pattern %s", temp, ig->names[match_pos]);
            free(temp);
            return 1;
        }

        match_pos = binary_search(slash_filename, ig->slash_names, 0, ig->slash_names_len);
        if (match_pos >= 0) {
            log_debug(L"file %s ignored because name matches slash static pattern %s", slash_filename, ig->slash_names[match_pos]);
            free(temp);
            return 1;
        }

        for (i = 0; i < ig->names_len; i++) {
			wchar_t *pos = wcsstr(slash_filename, ig->names[i]);
            if (pos == slash_filename || (pos && *(pos - 1) == '/')) {
                pos += wcslen(ig->names[i]);
                if (*pos == '\0' || *pos == '/') {
                    log_debug(L"file %s ignored because path somewhere matches name %s", slash_filename, ig->names[i]);
                    free(temp);
                    return 1;
                }
            }
            log_debug(L"pattern %s doesn't match path %s", ig->names[i], slash_filename);
        }

        for (i = 0; i < ig->slash_regexes_len; i++) {
            if (fnmatch(ig->slash_regexes[i], slash_filename, fnmatch_flags) == 0) {
                log_debug(L"file %s ignored because name matches slash regex pattern %s", slash_filename, ig->slash_regexes[i]);
                free(temp);
                return 1;
            }
            log_debug(L"pattern %s doesn't match slash file %s", ig->slash_regexes[i], slash_filename);
        }
    }

    for (i = 0; i < ig->regexes_len; i++) {
        if (fnmatch(ig->regexes[i], filename, fnmatch_flags) == 0) {
            log_debug(L"file %s ignored because name matches regex pattern %s", filename, ig->regexes[i]);
            free(temp);
            return 1;
        }
        log_debug(L"pattern %s doesn't match file %s", ig->regexes[i], filename);
    }

    int rv = ackmate_dir_match(temp);
    free(temp);
    return rv;
}

/* This function is REALLY HOT. It gets called for every file */
int filename_filter(const wchar_t *path, const struct dirent *dir, void *baton) {
	const wchar_t *filename = dir->d_name;
/* TODO: don't call strlen on filename every time we call filename_filter() */
#ifdef HAVE_DIRENT_DNAMLEN
    size_t filename_len = dir->d_namlen;
#else
    size_t filename_len = wcslen(filename);
#endif
    size_t i;
    scandir_baton_t *scandir_baton = (scandir_baton_t *)baton;
    const ignores *ig = scandir_baton->ig;
	const wchar_t *base_path = scandir_baton->base_path;
    const size_t base_path_len = scandir_baton->base_path_len;
	const wchar_t *path_start = path;
    wchar_t *temp;

    if (!opts.search_hidden_files && filename[0] == '.') {
        return 0;
    }

    for (i = 0; evil_hardcoded_ignore_files[i] != NULL; i++) {
        if (wcscmp(filename, evil_hardcoded_ignore_files[i]) == 0) {
            return 0;
        }
    }

    if (!opts.follow_symlinks && is_symlink(path, dir)) {
        log_debug(L"File %s ignored becaused it's a symlink", dir->d_name);
        return 0;
    }

    if (is_named_pipe(path, dir)) {
        log_debug(L"%s ignored because it's a named pipe", path);
        return 0;
    }

    if (opts.search_all_files && !opts.path_to_agignore) {
        return 1;
    }

    for (i = 0; base_path[i] == path[i] && i < base_path_len; i++) {
        /* base_path always ends with "/\0" while path doesn't, so this is safe */
        path_start = path + i + 2;
    }
    log_debug(L"path_start %s filename %s", path_start, filename);

    const wchar_t *extension = wcsrchr(filename, '.');
    if (extension) {
        if (extension[1]) {
            // The dot is not the last character, extension starts at the next one
            ++extension;
        } else {
            // No extension
            extension = NULL;
        }
    }

     while (ig != NULL) {
        if (wcsncmp(filename, L"./", 2) == 0) {
            filename++;
            filename_len--;
        }

        if (extension) {
            int match_pos = binary_search(extension, ig->extensions, 0, ig->extensions_len);
            if (match_pos >= 0) {
                log_debug(L"file %s ignored because name matches extension %s", filename, ig->extensions[match_pos]);
                return 0;
            }
        }

        if (path_ignore_search(ig, path_start, filename)) {
            return 0;
        }

        if (is_directory(path, dir) && filename[filename_len - 1] != '/') {
            ag_asprintf(&temp, L"%s/", filename);
            int rv = path_ignore_search(ig, path_start, temp);
            free(temp);
            if (rv) {
                return 0;
            }
        }
        ig = ig->parent;
    }

    log_debug(L"%s not ignored", filename);
    return 1;
}
