#ifndef LANG_H
#define LANG_H

#define MAX_EXTENSIONS 12
#define SINGLE_EXT_LEN 20

typedef struct {
    const wchar_t *name;
	const wchar_t *extensions[MAX_EXTENSIONS];
} lang_spec_t;

extern lang_spec_t langs[];

/**
 Return the language count.
 */
size_t get_lang_count(void);

/**
Convert a NULL-terminated array of language extensions
into a regular expression of the form \.(extension1|extension2...)$

Caller is responsible for freeing the returned string.
*/
wchar_t *make_lang_regex(wchar_t *ext_array, size_t num_exts);


/**
Combine multiple file type extensions into one array.

The combined result is returned through *exts*;
*exts* is one-dimension array, which can contain up to 100 extensions;
The number of extensions that *exts* actually contain is returned.
*/
size_t combine_file_extensions(size_t *extension_index, size_t len, wchar_t **exts);
#endif
