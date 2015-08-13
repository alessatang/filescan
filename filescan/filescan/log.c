#include <stdio.h>
#include <stdarg.h>

#include "log.h"
#include "util.h"

static enum log_level log_threshold = LOG_LEVEL_ERR;

void set_log_level(enum log_level threshold) {
    log_threshold = threshold;
}

void log_debug(const wchar_t *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vplog(LOG_LEVEL_DEBUG, fmt, args);
    va_end(args);
}

void log_msg(const wchar_t *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vplog(LOG_LEVEL_MSG, fmt, args);
    va_end(args);
}

void log_warn(const wchar_t *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vplog(LOG_LEVEL_WARN, fmt, args);
    va_end(args);
}

void log_err(const wchar_t *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vplog(LOG_LEVEL_ERR, fmt, args);
    va_end(args);
}

void vplog(const unsigned int level, const wchar_t *fmt, va_list args) {
    if ((enum log_level)level < log_threshold) {
        return;
    }

    FILE *stream = out_fd;

    switch (level) {
        case LOG_LEVEL_DEBUG:
            fwprintf(stream, L"DEBUG: ");
            break;
        case LOG_LEVEL_MSG:
            fwprintf(stream, L"MSG: ");
            break;
        case LOG_LEVEL_WARN:
            fwprintf(stream, L"WARN: ");
            break;
        case LOG_LEVEL_ERR:
            stream = stderr;
            fwprintf(stream, L"ERR: ");
            break;
    }

    vfwprintf(stream, fmt, args);
    fwprintf(stream, L"\n");
}

void plog(const unsigned int level, const wchar_t *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vplog(level, fmt, args);
    va_end(args);
}
