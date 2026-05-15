/* arena - Arena memory allocator
 *
 * Wraps malloc/realloc/free from stdlib.h into an arena allocator.
 * Uses a byte offset to track the position of allocated/free memory.
 */

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    size_t size;
    size_t offset;
    uint8_t *buffer;
} Arena;

Arena *arena_create(size_t size)
{
    Arena *a;
    if ((a = malloc(sizeof(Arena))) == NULL) {
        return NULL;
    }
    if ((a->buffer = malloc(size)) == NULL) {
        free(a);
        return NULL;
    }

    a->size = size;
    a->offset = 0;
    return a;
}

void arena_free(Arena *a)
{
    if (a == NULL) {
        return;
    }
    free(a->buffer);
    free(a);
}

void *arena_malloc(Arena *a, size_t size)
{
    if (a->offset + size > a->size) {
        return NULL;
    }

    uint8_t *ptr = a->buffer + a->offset;
    a->offset += size;
    return ptr;
}

void *arena_calloc(Arena *a, size_t size)
{
    if (a->offset + size > a->size) {
        return NULL;
    }

    uint8_t *ptr = a->buffer + a->offset;
    a->offset += size;
    for (size_t i = 0; i < size; i++) {
        ptr[i] = 0;
    }
    return ptr;
}

void arena_clear(Arena *a)
{
    for (size_t i = 0; i < a->size; i++) {
        a->buffer[i] = 0;
    }
}

size_t arena_remaining(Arena *a)
{
    return a->size - a->offset;
}

void arena_reset(Arena *a)
{
    a->offset = 0;
}

void *arena_memcpy(void *restrict dst, const void *restrict src, size_t n)
{
    uint8_t *d = (uint8_t *)dst;
    const uint8_t *s = (const uint8_t *)src;

    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dst;
}

void *arena_memdup(Arena *a, const void *src, size_t n)
{
    void *dup = arena_malloc(a, n);
    return arena_memcpy(dup, src, n);
}

char *arena_strcpy(char *restrict dst, const char *restrict src)
{
    char *ret = dst;
    while ((*dst++ = *src++)) {
        continue;
    }
    return ret;
}

char *arena_strdup(Arena *a, const char *src)
{
    size_t len = strlen(src);
    return arena_memdup(a, src, len + 1);
}

char *arena_sprintf(Arena *a, const char *format, ...)
{
    va_list args;
    size_t len = 0;
    char *buf = NULL;

    va_start(args, format);
    len = vsnprintf(buf, len, format, args);
    va_end(args);

    if (len < 0) {
        return NULL;
    }

    buf = arena_malloc(a, len + 1);
    va_start(args, format);
    vsnprintf(buf, len + 1, format, args);
    va_end(args);

    return buf;
}

