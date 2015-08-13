#ifndef SCANDIR_H
#define SCANDIR_H

#include "ignore.h"

typedef struct {
    const ignores *ig;
	const wchar_t *base_path;
    size_t base_path_len;
} scandir_baton_t;

typedef int (*filter_fp)(const wchar_t *path, const struct dirent *, void *);

int ag_scandir(const wchar_t *dirname,
               struct dirent ***namelist,
               filter_fp filter,
               void *baton);

#endif
