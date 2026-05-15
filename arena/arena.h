/* arena.c/h - Arena memory allocator
 *
 * Wraps malloc/realloc/free from stdlib.h into an Arena allocator.
 * Uses a byte offset to track the position of allocated/free memory.
 */

#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>

typedef struct Arena Arena;

Arena   *arena_create(size_t size);
void     arena_free(Arena *a);

void    *arena_malloc(Arena *a, size_t size);
void    *arena_calloc(Arena *a, size_t size);
void     arena_clear(Arena *a);
size_t   arena_remaining(Arena *a);
void     arena_reset(Arena *a);

void    *arena_memcpy(void *restrict dst, const void *restrict src, size_t n);
void    *arena_memdup(Arena *a, const void *src, size_t n);
char    *arena_strcpy(char *restrict dst, const char *restrict src);
char    *arena_strdup(Arena *a, const char *src);
char    *arena_sprintf(Arena *a, const char *format, ...);

#endif

