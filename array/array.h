/*
 * Dynamic Array - single header library
 */

#ifndef ARRAY_H
#define ARRAY_H

#include <stdbool.h>
#include <stddef.h>

#define ARRAY_PUSH(arr, val) (array_push((arr), &(val)))
#define ARRAY_POP(arr, type) (*(type *)array_pop(arr))
#define ARRAY_GET(arr, type, idx) (*(type *)array_get(arr, idx))

typedef struct Array {
    void *buffer;
    size_t elem_size;
    size_t size;
    size_t cap;
} Array;

Array *array_create(size_t elem_size);
void   array_free(Array *array);
bool   array_push(Array *array, void *value);
void  *array_pop(Array *array);
void  *array_get(Array *array, size_t idx);

#ifdef ARRAY_IMPLEMENTATION
#ifndef IMPLEMENTATION_GAURD
#define IMPLEMENTATION_GAURD

#include <stdint.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 16

Array *array_create(size_t elem_size)
{
    Array *array = (Array *)malloc(sizeof(Array));
    if (array == NULL) return NULL;

    array->buffer = malloc(elem_size * INITIAL_CAPACITY);
    if (array->buffer == NULL) {
        free(array);
        return NULL;
    }

    array->size = 0;
    array->elem_size = elem_size;
    array->cap = INITIAL_CAPACITY;

    return array;
}

void array_free(Array *array)
{
    if (array == NULL) return;

    if (array->buffer != NULL) free(array->buffer);
    free(array);
}

static bool resize_array(Array *array)
{
    void *temp = realloc(array->buffer, array->cap * 2);
    if (temp == NULL) return false;

    array->buffer = temp;
    array->cap *= 2;

    return true;
}

bool array_push(Array *array, void *value)
{
    if (array->size * array->elem_size >= array->cap) {
        if (!resize_array(array)) return false;
    }

    uint8_t *buffer = (uint8_t *)array->buffer;
    size_t offset = array->size * array->elem_size;
    buffer += offset;

    for (size_t i = 0; i < array->elem_size; i++) {
        *buffer++ = ((uint8_t *)value)[i];
    }

    array->size++;

    return true;
}

void *array_pop(Array *array)
{
    array->size--;
    uint8_t *buffer = (uint8_t *)array->buffer;
    size_t offset = array->size * array->elem_size;

    return buffer + offset;
}

void *array_get(Array *array, size_t idx)
{
    uint8_t *buffer = (uint8_t *)array->buffer;
    size_t offset = idx * array->elem_size;

    return buffer + offset;
}

#endif
#endif
#endif

