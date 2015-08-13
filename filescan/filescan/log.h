#ifndef LOG_H
#define LOG_H

#include <stdarg.h>

enum log_level {
    LOG_LEVEL_DEBUG = 10,
    LOG_LEVEL_MSG = 20,
    LOG_LEVEL_WARN = 30,
    LOG_LEVEL_ERR = 40,
    LOG_LEVEL_NONE = 100
};

void set_log_level(enum log_level threshold);

void log_debug(const wchar_t *fmt, ...);
void log_msg(const wchar_t *fmt, ...);
void log_warn(const wchar_t *fmt, ...);
void log_err(const wchar_t *fmt, ...);

void vplog(const unsigned int level, const wchar_t *fmt, va_list args);
void plog(const unsigned int level, const wchar_t *fmt, ...);

#endif
