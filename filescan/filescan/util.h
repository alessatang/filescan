#ifndef UTIL_H
#define UTIL_H

#include "dirent.h"
#include <pcre.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include "utime.h"

#include "config.h"
#include "log.h"
#include "options.h"

extern FILE *out_fd;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

void *ag_malloc(size_t size);
void *ag_realloc(void *ptr, size_t size);
void *ag_calloc(size_t nelem, size_t elsize);
wchar_t *ag_strdup(const wchar_t *s);
wchar_t *ag_strndup(const wchar_t *s, size_t size);

typedef struct {
    size_t start; /* Byte at which the match starts */
    size_t end;   /* and where it ends */
} match_t;

typedef struct {
    long total_bytes;
    long total_files;
    long total_matches;
    struct timeval time_start;
    struct timeval time_end;
} ag_stats;

typedef enum {
    AG_NO_COMPRESSION,
    AG_GZIP,
    AG_COMPRESS,
    AG_ZIP
} ag_compression_type;

extern ag_stats stats;


typedef const wchar_t *(*strncmp_fp)(const wchar_t *, const wchar_t *, const size_t, const size_t, const size_t[], const size_t *);

void free_strings(wchar_t **strs, const size_t strs_len);

void generate_alpha_skip(const wchar_t *find, size_t f_len, size_t skip_lookup[], const int case_sensitive);
int is_prefix(const wchar_t *s, const size_t s_len, const size_t pos, const int case_sensitive);
size_t suffix_len(const wchar_t *s, const size_t s_len, const size_t pos, const int case_sensitive);
void generate_find_skip(const wchar_t *find, const size_t f_len, size_t **skip_lookup, const int case_sensitive);

/* max is already defined on spec-violating compilers such as MinGW */
size_t ag_max(size_t a, size_t b);

const wchar_t *boyer_moore_strnstr(const wchar_t *s, const wchar_t *find, const size_t s_len, const size_t f_len,
                                const size_t alpha_skip_lookup[], const size_t *find_skip_lookup);
const wchar_t *boyer_moore_strncasestr(const wchar_t *s, const wchar_t *find, const size_t s_len, const size_t f_len,
                                    const size_t alpha_skip_lookup[], const size_t *find_skip_lookup);

strncmp_fp get_strstr(enum case_behavior opts);

size_t invert_matches(const wchar_t *buf, const size_t buf_len, match_t matches[], size_t matches_len);
void compile_study(pcre16 **re, pcre16_extra **re_extra, wchar_t *q, const int pcre_opts, const int study_opts);

void *decompress(const ag_compression_type zip_type, const void *buf, const int buf_len, const wchar_t *dir_full_path, int *new_buf_len);
ag_compression_type is_zipped(const void *buf, const int buf_len);

int is_binary(const wchar_t *buf, const size_t buf_len);
int is_regex(const wchar_t *query);
int is_fnmatch(const wchar_t *filename);
int binary_search(const wchar_t *needle, wchar_t **haystack, int start, int end);

void init_wordchar_table(void);
int is_wordchar(wchar_t ch);

int is_lowercase(const wchar_t *s);

int is_directory(const wchar_t *path, const struct dirent *d);
int is_symlink(const wchar_t *path, const struct dirent *d);
int is_named_pipe(const wchar_t *path, const struct dirent *d);

void die(const wchar_t *fmt, ...);

void ag_asprintf(wchar_t **ret, const wchar_t *fmt, ...);
int Unicode2UTF8(const wchar_t *pSource, char** pDest, int *destSize);
int UTF82Unicode(const char *pSource, wchar_t **pDest, int *destSize);
int getTempFile(wchar_t *tmp_file);
int isFilePathValid(const wchar_t *path);

#ifndef HAVE_FGETLN
wchar_t *fgetln(FILE *fp, size_t *lenp);
#endif
#ifndef HAVE_GETLINE
ssize_t getline(wchar_t **lineptr, size_t *n, FILE *stream);
#endif
#ifndef HAVE_REALPATH
wchar_t *realpath(const wchar_t *path, wchar_t *resolved_path);
#endif
#ifndef HAVE_STRLCPY
size_t strlcpy(wchar_t *dest, const wchar_t *src, size_t size);
#endif
#ifndef HAVE_VASPRINTF
int vasprintf(wchar_t **ret, const wchar_t *fmt, va_list args);
#endif

#endif
