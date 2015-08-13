#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "ignore.h"
#include "log.h"
#include "options.h"
#include "print.h"
#include "util.h"
#ifdef _WIN32
#define fprintf(...) fprintf_w32(__VA_ARGS__)
#endif

int first_file_match = 1;

const wchar_t *color_reset = L"\033[0m\033[K";


#ifdef _WIN32
#include <windows.h>
static HANDLE console_handle = NULL;
static WORD default_attr = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN;

static int get_console_handle(void) {
    CONSOLE_SCREEN_BUFFER_INFO buf;
    BOOL ok;
    if (console_handle != NULL)
        return 1;
    console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!console_handle)
        return 0;
    ok = GetConsoleScreenBufferInfo(console_handle, &buf);
    if (ok) {
        default_attr = buf.wAttributes;
    }
    return 1;
}

void set_output_color(WORD c) {
    if (!get_console_handle())
        return;
    SetConsoleTextAttribute(console_handle, c);
}

void color_highlight_path(FILE *out_fd) { set_output_color(FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); }
void color_highlight_match(FILE *out_fd) { set_output_color(FOREGROUND_BLUE |                  FOREGROUND_GREEN | FOREGROUND_INTENSITY ); }
void color_highlight_line_no(FILE *out_fd) { set_output_color(                  FOREGROUND_RED |                    FOREGROUND_INTENSITY ); }
void color_normal(FILE *out_fd) {
    get_console_handle();
    set_output_color(default_attr);
}
#else
void color_highlight_path   (FILE *out_fd) { fprintf(out_fd, "%s", opts.color_path       ); }
void color_highlight_match  (FILE *out_fd) { fprintf(out_fd, "%s", opts.color_match      ); }
void color_highlight_line_no(FILE *out_fd) { fprintf(out_fd, "%s", opts.color_line_number); }
void color_normal           (FILE *out_fd) { fprintf(out_fd, "%s", color_reset           ); }
#endif


void print_path(const wchar_t *path, const wchar_t sep) {
    path = normalize_path(path);
	const wchar_t *buf = fix_path_slashes(path);

    if (opts.ackmate) {
        fprintf(out_fd, L":%s%c", buf, sep);
        fprintf(out_fd, L":%s%c", path, sep);
    } else if (opts.vimgrep) {
        fprintf(out_fd, L"%s%c", path, sep);
    } else {
        if (opts.color) {
            color_highlight_path(out_fd);
            fprintf(out_fd, L"%s", buf);
            color_normal(out_fd);
            fprintf(out_fd, L"%c", sep);
        } else {
            fprintf(out_fd, L"%s%c", buf, sep);
        }
    }
    free((void*)buf);
}

void print_path_count(const wchar_t *path, const wchar_t sep, const size_t count) {
    if (*path) {
        print_path(path, ':');
    }
    if (opts.color) {
        fprintf(out_fd, L"%s%lu%s%c", opts.color_line_number, (unsigned long)count, color_reset, sep);
    } else {
        fprintf(out_fd, L"%lu%c", (unsigned long)count, sep);
    }
}

void print_line(const wchar_t *buf, size_t buf_pos, size_t prev_line_offset) {
    fwrite(buf + prev_line_offset, 1, buf_pos - prev_line_offset + 1, out_fd);
}

void print_binary_file_matches(const wchar_t *path) {
    path = normalize_path(path);
	const wchar_t *buf = fix_path_slashes(path);
    print_file_separator();
    fprintf(out_fd, L"Binary file %s matches.\n", buf);
    free((void*)buf);
}

void print_file_matches(const wchar_t *path, const wchar_t *buf, const size_t buf_len, const match_t matches[], const size_t matches_len) {
    size_t line = 1;
	wchar_t **context_prev_lines = NULL;
    size_t prev_line = 0;
    size_t last_prev_line = 0;
    size_t prev_line_offset = 0;
    size_t cur_match = 0;
    size_t lines_since_last_match = INT_MAX;
    ssize_t lines_to_print = 0;
    size_t last_printed_match = 0;
	wchar_t sep = '-';
    size_t i, j;
    int in_a_match = FALSE;
    int printing_a_match = FALSE;

    if (opts.ackmate || opts.vimgrep) {
        sep = ':';
    }

    print_file_separator();

    if (opts.print_path == PATH_PRINT_DEFAULT) {
        opts.print_path = PATH_PRINT_TOP;
    } else if (opts.print_path == PATH_PRINT_DEFAULT_EACH_LINE) {
        opts.print_path = PATH_PRINT_EACH_LINE;
    }

    if (opts.print_path == PATH_PRINT_TOP) {
        if (opts.print_count) {
            print_path_count(path, opts.path_sep, matches_len);
        } else {
            print_path(path, opts.path_sep);
        }
    }

	context_prev_lines = (wchar_t **)ag_calloc(sizeof(wchar_t *), (opts.before + 1));

    for (i = 0; i <= buf_len && (cur_match < matches_len || lines_since_last_match <= opts.after); i++) {
        if (cur_match < matches_len && i == matches[cur_match].start) {
            in_a_match = TRUE;
            /* We found the start of a match */
            if (cur_match > 0 && opts.context && lines_since_last_match > (opts.before + opts.after + 1)) {
                fprintf(out_fd, L"--\n");
            }

            if (lines_since_last_match > 0 && opts.before > 0) {
                /* TODO: better, but still needs work */
                /* print the previous line(s) */
                lines_to_print = lines_since_last_match - (opts.after + 1);
                if (lines_to_print < 0) {
                    lines_to_print = 0;
                } else if ((size_t)lines_to_print > opts.before) {
                    lines_to_print = opts.before;
                }

                for (j = (opts.before - lines_to_print); j < opts.before; j++) {
                    prev_line = (last_prev_line + j) % opts.before;
                    if (context_prev_lines[prev_line] != NULL) {
                        if (opts.print_path == PATH_PRINT_EACH_LINE) {
                            print_path(path, ':');
                        }
                        print_line_number(line - (opts.before - j), sep);
                        fprintf(out_fd, L"%s\n", context_prev_lines[prev_line]);
                    }
                }
            }
            lines_since_last_match = 0;
        }

        if (cur_match < matches_len && i == matches[cur_match].end) {
            /* We found the end of a match. */
            cur_match++;
            in_a_match = FALSE;
        }

        /* We found the end of a line. */
        if (buf[i] == '\n' && opts.before > 0) {
            if (context_prev_lines[last_prev_line] != NULL) {
                free(context_prev_lines[last_prev_line]);
            }
            /* We don't want to strcpy the \n */
            context_prev_lines[last_prev_line] = ag_strndup(&buf[prev_line_offset], i - prev_line_offset);
            last_prev_line = (last_prev_line + 1) % opts.before;
        }

        if (buf[i] == '\n' || i == buf_len) {
            if (lines_since_last_match == 0) {
                if (opts.print_path == PATH_PRINT_EACH_LINE && !opts.search_stream) {
                    print_path(path, ':');
                }
                if (opts.ackmate) {
                    /* print headers for ackmate to parse */
                    print_line_number(line, ';');
                    for (; last_printed_match < cur_match; last_printed_match++) {
                        fprintf(out_fd, L"%lu %lu",
                                (unsigned long)(matches[last_printed_match].start - prev_line_offset),
                                (unsigned long)(matches[last_printed_match].end - matches[last_printed_match].start));
                        last_printed_match == cur_match - 1 ? fputc(':', out_fd) : fputc(',', out_fd);
                    }
                    print_line(buf, i, prev_line_offset);
                } else if (opts.vimgrep) {
                    for (; last_printed_match < cur_match; last_printed_match++) {
                        print_path(path, sep);
                        print_line_number(line, sep);
                        print_column_number(matches, last_printed_match, prev_line_offset, sep);
                        print_line(buf, i, prev_line_offset);
                    }
                } else {
                    print_line_number(line, ':');
                    int printed_match = FALSE;
                    if (opts.column) {
                        print_column_number(matches, last_printed_match, prev_line_offset, ':');
                    }

                    if (printing_a_match && opts.color) {
                        color_highlight_match(out_fd);
                    }
                    for (j = prev_line_offset; j <= i; j++) {
                        if (last_printed_match < matches_len && j == matches[last_printed_match].end) {
                            if (opts.color) {
                                color_normal(out_fd);
                            }
                            printing_a_match = FALSE;
                            last_printed_match++;
                            printed_match = TRUE;
                            if (opts.only_matching) {
                                fputc('\n', out_fd);
                            }
                        }
                        if (last_printed_match < matches_len && j == matches[last_printed_match].start) {
                            if (opts.only_matching && printed_match) {
                                if (opts.print_path == PATH_PRINT_EACH_LINE) {
                                    print_path(path, ':');
                                }
                                print_line_number(line, ':');
                                if (opts.column) {
                                    print_column_number(matches, last_printed_match, prev_line_offset, ':');
                                }
                            }
                            if (opts.color) {
                                color_highlight_match(out_fd);
                            }
                            printing_a_match = TRUE;
                        }
                        /* Don't print the null terminator */
                        if (j < buf_len) {
                            /* if only_matching is set, print only matches and newlines */
                            if (!opts.only_matching || printing_a_match) {
                                fputwc(buf[j], out_fd);
                            }
                        }
                    }
                    if (printing_a_match && opts.color) {
                        color_normal(out_fd);
                    }
                }
            } else if (lines_since_last_match <= opts.after) {
                /* print context after matching line */
                if (opts.print_path == PATH_PRINT_EACH_LINE) {
                    print_path(path, ':');
                }
                print_line_number(line, sep);

                for (j = prev_line_offset; j < i; j++) {
                    fputwc(buf[j], out_fd);
                }
                fputwc('\n', out_fd);
            }

            prev_line_offset = i + 1; /* skip the newline */
            line++;
            if (!in_a_match && lines_since_last_match < INT_MAX) {
                lines_since_last_match++;
            }
            /* File doesn't end with a newline. Print one so the output is pretty. */
            if (i == buf_len && buf[i] != '\n' && !opts.search_stream) {
                fputwc('\n', out_fd);
            }
        }
    }

    for (i = 0; i < opts.before; i++) {
        if (context_prev_lines[i] != NULL) {
            free(context_prev_lines[i]);
        }
    }
    free(context_prev_lines);
}

void print_line_number(size_t line, const wchar_t sep) {
    if (!opts.print_line_numbers) {
        return;
    }
    if (opts.search_stream && opts.stream_line_num) {
        line = opts.stream_line_num;
    }
    if (opts.color) {
        fprintf(out_fd, L"%s%lu%s%c", opts.color_line_number, (unsigned long)line, color_reset, sep);
    } else {
        fprintf(out_fd, L"%lu%c", (unsigned long)line, sep);
    }

}

void print_column_number(const match_t matches[], size_t last_printed_match,
                         size_t prev_line_offset, const wchar_t sep) {
    fprintf(out_fd, L"%lu%c",
            (unsigned long)(matches[last_printed_match].start - prev_line_offset) + 1,
            sep);
}

void print_file_separator(void) {
    if (first_file_match == 0 && opts.print_break) {
        fprintf(out_fd, L"\n");
    }
    first_file_match = 0;
}

const wchar_t *normalize_path(const wchar_t *path) {
    if (wcslen(path) < 3) {
        return path;
    }
    if (path[0] == '.' && path[1] == '/') {
        return path + 2;
    }
    if (path[0] == '/' && path[1] == '/') {
        return path + 1;
    }
    return path;
}

const wchar_t* fix_path_slashes(const wchar_t* path) {
	wchar_t* buf = _wcsdup(path);
#ifdef _WIN32
	for (wchar_t* i = buf; *i; ++i)
    {
        if(*i == '/') *i = '\\';
    }
#endif
    return buf;
}
