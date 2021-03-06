#ifndef IGNORE_H
#define IGNORE_H

#include "dirent.h"
#include <sys/types.h>

#define SVN_DIR_PROP_BASE L"dir-prop-base"
#define SVN_DIR L".svn"
#define SVN_PROP_IGNORE L"svn:ignore"

struct ignores {
	wchar_t **extensions; /* File extensions to ignore */
    size_t extensions_len;

	wchar_t **names; /* Non-regex ignore lines. Sorted so we can binary search them. */
    size_t names_len;
	wchar_t **slash_names; /* Same but starts with a slash */
    size_t slash_names_len;

	wchar_t **regexes; /* For patterns that need fnmatch */
    size_t regexes_len;
	wchar_t **slash_regexes;
    size_t slash_regexes_len;

	const wchar_t *dirname;
    size_t dirname_len;
	wchar_t *abs_path;
    size_t abs_path_len;

    struct ignores *parent;
};
typedef struct ignores ignores;

extern ignores *root_ignores;

extern const wchar_t *evil_hardcoded_ignore_files[];
extern const wchar_t *ignore_pattern_files[];

ignores *init_ignore(ignores *parent, const wchar_t *dirname, const size_t dirname_len);
void cleanup_ignore(ignores *ig);

void add_ignore_pattern(ignores *ig, const wchar_t *pattern);

void load_ignore_patterns(ignores *ig, const wchar_t *path);
void load_svn_ignore_patterns(ignores *ig, const wchar_t *path);

int filename_filter(const wchar_t *path, const struct dirent *dir, void *baton);

int is_empty(ignores *ig);

#endif
