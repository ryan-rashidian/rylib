/*
 * array - Dynamic Array
 */

#ifndef ARRAY_H
#define ARRAY_H

#include <stdbool.h>
#include <stddef.h>

#ifndef ARRAY_VALUE_TYPE
#define ARRAY_VALUE_TYPE double
#endif

typedef ARRAY_VALUE_TYPE AValue;

typedef struct Array Array;

Array *array_init(void);
void array_free(Array *arr);

bool array_push(Array *arr, AValue value);
bool array_set(Array *arr, size_t index, AValue value);
bool array_get(Array *arr, size_t index, AValue *value);

AValue *array_iter_start(Array *arr);
AValue *array_iter_end(Array *arr);

size_t array_count(Array *arr);

#ifdef ARRAY_IMPLEMENTATION
#ifndef ARRAY_IMPL_GAURD
#define ARRAY_IMPL_GAURD

#include <stdlib.h>

#define ARRAY_GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity) * 2)

struct Array {
    size_t count;
    size_t capacity;
    AValue *buffer;
};

Array *array_init(void)
{
    Array *arr = (Array *)malloc(sizeof(Array));
    if (arr == NULL) return NULL;

    arr->count = 0;
    arr->capacity = 0;
    arr->buffer = NULL;

    return arr;
}

void array_free(Array *arr)
{
    if (arr == NULL) return;

    if (arr->buffer != NULL) free(arr->buffer);
    free(arr);
}

static bool grow_array(Array *arr, size_t capacity)
{
    size_t new_size = sizeof(AValue) * capacity;
    AValue *new_buffer = (AValue *)realloc(arr->buffer, new_size);
    if (new_buffer == NULL) return false;

    arr->buffer = new_buffer;
    arr->capacity = capacity;

    return true;
}

bool array_push(Array *arr, AValue value)
{
    if (arr->capacity < arr->count + 1) {
        size_t new_capacity = ARRAY_GROW_CAPACITY(arr->capacity);
        if (!grow_array(arr, new_capacity)) return false;
    }

    arr->buffer[arr->count++] = value;

    return true;
}

bool array_set(Array *arr, size_t index, AValue value)
{
    if (arr->count == 0 || arr->count - 1 < index) return false;

    arr->buffer[index] = value;

    return true;
}

bool array_get(Array *arr, size_t index, AValue *value)
{
    if (arr->count == 0 || arr->count - 1 < index) return false;

    *value = arr->buffer[index];

    return true;
}

AValue *array_iter_start(Array *arr)
{
    return arr->buffer;
}

AValue *array_iter_end(Array *arr)
{
    return arr->buffer + arr->count;
}

size_t array_count(Array *arr)
{
    return arr->count;
}

#endif
#endif
#endif

