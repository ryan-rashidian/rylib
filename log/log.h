/* 
 * log.h - Logging utility
 */

#ifndef LOG_H
#define LOG_H

#define LOG(l , ...) logger((l), __VA_ARGS__)

enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
};

void logger(int level, const char *message, ...);

#endif

#ifdef LOG_IMPLEMENTATION
#ifndef LOG_IMPL_GUARD
#define LOG_IMPL_GUARD

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define LOG_BUFFER_MAX 256

static int log_level = LOG_INFO;

static char *log_level_strings[] = {
    "[LOG]: DEBUG: ",
    "[LOG]: INFO: ",
    "[LOG]: WARNING: ",
    "[LOG]: ERROR: ",
};

void logger(int level, const char *message, ...)
{
    if (level < log_level || message == NULL) return;

    char buffer[LOG_BUFFER_MAX] = { 0 };
    size_t lvl_len = strlen(log_level_strings[level]);
    size_t msg_len_max = LOG_BUFFER_MAX - lvl_len - 2;

    memcpy(buffer, log_level_strings[level], lvl_len);
    strncat(buffer, message, msg_len_max);
    strcat(buffer, "\n");
    
    va_list va;
    va_start(va, message);
    vfprintf(stderr, buffer, va);
    va_end(va);
}

#endif
#endif

