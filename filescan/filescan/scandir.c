#include "dirent.h"
#include <stdlib.h>

#include "scandir.h"
#include "util.h"

int ag_scandir(const wchar_t *dirname,
               struct dirent ***namelist,
               filter_fp filter,
               void *baton) {
    DIR *dirp = NULL;
    struct dirent **names = NULL;
    struct dirent *entry, *d;
    int names_len = 32;
    int results_len = 0;

    dirp = opendir(dirname);
    if (dirp == NULL) {
        goto fail;
    }

    names = (struct dirent**)malloc(sizeof(struct dirent*) * names_len);
    if (names == NULL) {
        goto fail;
    }

    while ((entry = readdir(dirp)) != NULL) {
        if ((*filter)(dirname, entry, baton) == FALSE) {
            continue;
        }
        if (results_len >= names_len) {
            struct dirent **tmp_names = names;
            names_len *= 2;
            names = (struct dirent**)realloc(names, sizeof(struct dirent*) * names_len);
            if (names == NULL) {
                free(tmp_names);
                goto fail;
            }
        }


#if defined _MSC_VER
        size_t s_len = wcslen(entry->d_name) + 1;
        d = (struct dirent *)malloc(sizeof(struct dirent));
		wchar_t *s = (wchar_t *)malloc(sizeof(wchar_t) * s_len);
        d->d_name = s;
        memcpy(s, entry->d_name, s_len*sizeof(wchar_t));
#else

#if defined(__MINGW32__) || defined(__CYGWIN__)
        d = malloc(sizeof(struct dirent));
#else
        d = malloc(entry->d_reclen);
#endif

        if (d == NULL) {
            goto fail;
        }
#if defined(__MINGW32__) || defined(__CYGWIN__)
        memcpy(d, entry, sizeof(struct dirent));
#else
        memcpy(d, entry, entry->d_reclen);
#endif

        
#endif /* _MSC_VER */

        names[results_len] = d;
        results_len++;
    }

    closedir(dirp);
    *namelist = names;
    return results_len;

fail:
    if (dirp) {
        closedir(dirp);
    }

    if (names != NULL) {
        int i;
        for (i = 0; i < results_len; i++) {
            free(names[i]);
        }
        free(names);
    }
    return -1;
}
