/*
 * vstring - String viewer
 *
 * Viewer interface for read only string constants. 
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vstring.h"

VString vstring_create(const char *data, size_t len)
{
    if (data == NULL) return VS_NULL;
    if (len == 0) len = strlen(data);

    VString vs;
    vs.len = len;
    vs.buffer = data;

    return vs;
}

size_t vstring_find_count(VString *vs, char c)
{
    size_t count = 0;

    for (size_t i = 0; i < vs->len; i++) {
        if (vs->buffer[i] == c) count++;
    }

    return count;
}

size_t vstring_find_first(VString *vs, char c)
{
    size_t i = 0;

    while (i < vs->len && vs->buffer[i] != c) i++;

    if (vs->buffer[i] == c) return i;
    else return vs->len + 1;
}

size_t vstring_find_last(VString *vs, char c)
{
    size_t i = vs->len - 1;

    while (i > 0 && vs->buffer[i] != c) i--;

    if (vs->buffer[i] == c) return i;
    else return vs->len + 1;
}

void vstring_trim_left(VString *vs, char c)
{
    size_t i = 0;

    while (i < vs->len && vs->buffer[i] == c) i++;

    vs->buffer = &vs->buffer[i];
    vs->len -= i;
}

void vstring_trim_right(VString *vs, char c)
{
    while (vs->len > 0 && vs->buffer[vs->len-1] == c) vs->len--;
}

void vstring_trim(VString *vs, char c)
{
    vstring_trim_left(vs, c);
    vstring_trim_right(vs, c);
}

void vstring_trim_space_left(VString *vs)
{
    size_t i = 0;

    while (i < vs->len && isspace(vs->buffer[i])) i++;

    vs->buffer = &vs->buffer[i];
    vs->len -= i;
}

void vstring_trim_space_right(VString *vs)
{
    while (vs->len > 0 && isspace(vs->buffer[vs->len-1])) vs->len--;
}

void vstring_trim_space(VString *vs)
{
    vstring_trim_space_left(vs);
    vstring_trim_space_right(vs);
}

void vstring_slice(VString *vs, size_t start, size_t end)
{
    if (start >= vs->len || end >= vs->len || start > end) return;

    vs->buffer += start;
    vs->len = end - (start - 1);
}

static size_t slice_char(VString *vs, char c)
{
    size_t i;

    for (i = 0; i < vs->len; i++) {
        if (vs->buffer[i] == c) return i;
    }

    return i;
}

void vstring_slice_char_left(VString *vs, char c)
{
    size_t i = slice_char(vs, c);
    if (i >= vs->len) return;

    vs->len = i + 1;
}

void vstring_slice_char_right(VString *vs, char c)
{
    size_t i = slice_char(vs, c);
    if (i >= vs->len) return;

    vs->buffer = &vs->buffer[i];
    vs->len -= i;
}

void vstring_slice_char(VString *vs, char start, char end)
{
    vstring_slice_char_left(vs, end);
    vstring_slice_char_right(vs, start);
}

