#ifndef PRINT_H
#define PRINT_H

#include "util.h"


void print_path(const wchar_t* path, const wchar_t sep);
void print_path_count(const wchar_t *path, const wchar_t sep, const size_t count);
void print_line(const wchar_t *buf, size_t buf_pos, size_t prev_line_offset);
void print_binary_file_matches(const wchar_t* path);
void print_file_matches(const wchar_t* path, const wchar_t* buf, const size_t buf_len, const match_t matches[], const size_t matches_len);
void print_line_number(size_t line, const wchar_t sep);
void print_column_number(const match_t matches[], size_t last_printed_match,
	size_t prev_line_offset, const wchar_t sep);
void print_file_separator(void);

const wchar_t *normalize_path(const wchar_t *path);
const wchar_t *fix_path_slashes(const wchar_t* path);

#ifdef _WIN32
void windows_use_ansi(int use_ansi);
int fprintf_w32(FILE *fp, const wchar_t *format, ...);
#endif
#endif
