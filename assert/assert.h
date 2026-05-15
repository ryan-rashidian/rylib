/* assert - Assert macro */

#ifndef ASSERT_H
#define ASSERT_H

#if ASSERT_ENABLED

#include <stdio.h>
#include <stdlib.h>

#define ASSERT(c, m) \
do { \
    if (!(c)) { \
        fprintf(stderr, "%s: line %d: assertion %s failed: %s\n", \
                        __FILE__, __LINE__, #c, m); \
        exit(1); \
    } \
} while(0)

#else
#define ASSERT(c, m) // empty macro if disabled
#endif

#endif

