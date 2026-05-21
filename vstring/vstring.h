/*
 * vstring - String viewer
 *
 * Viewer interface for read only string constants. 
 */

#ifndef VSTRING_H
#define VSTRING_H

#include <stddef.h>

#define VS_NULL ((VString){ .buffer = NULL, .len = 0 })
#define VS_IS_NULL(vs) ((vs).buffer == NULL && (vs).len == 0)
#define VS_PRINT(vs) (int)(vs).len, (vs).buffer

typedef struct VString {
    size_t len;
    const char *buffer;
} VString;

VString vstring_create(const char *data, size_t len);

size_t  vstring_find_count(VString *vs, char c);
size_t  vstring_find_first(VString *vs, char c);
size_t  vstring_find_last(VString *vs, char c);

void    vstring_trim_left(VString *vs, char c);
void    vstring_trim_right(VString *vs, char c);
void    vstring_trim(VString *vs, char c);
void    vstring_trim_space_left(VString *vs);
void    vstring_trim_space_right(VString *vs);
void    vstring_trim_space(VString *vs);

void    vstring_slice(VString *vs, size_t start, size_t end);
void    vstring_slice_char_left(VString *vs, char c);
void    vstring_slice_char_right(VString *vs, char c);
void    vstring_slice_char(VString *vs, char start, char end);

#endif

