/* 
 * log - Logging macro
 */

#ifndef LOG_H
#define LOG_H

#if LOG_ENABLED

#include <stdio.h>
#include <stddef.h>

#define LOG(f, m) \
do { \
    FILE *fp = fopen(f, "ab"); \
    if (fp != NULL) { \
        fprintf(fp, "%s: line %d: LOG msg: %s\n", \
                    __FILE__, __LINE__, m); \
    } \
    fclose(fp); \
} while(0)

#else
#define LOG(f, m) // empty macro if disabled
#endif

#endif

