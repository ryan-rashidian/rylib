/* 
 * arr.c/h - Dynamic array
 *
 * Arr struct is created with a starting `capacity`. When the array is full,
 * it will automatically resize by doubling its current capacity. The array's
 * type is implied by the given `elem_size`.
 */

#ifndef ARR_H
#define ARR_H

#include <stddef.h>

typedef struct Arr Arr;

Arr     *arr_create(size_t elem_size, size_t capacity);
void     arr_free(Arr *arr);

void     arr_clear(Arr *arr);
void    *arr_reserve(Arr *arr, size_t capacity);

void    *arr_push(Arr *arr, void *elem);
void    *arr_pop(Arr *arr);
void    *arr_get(Arr *arr, int index);
size_t   arr_size(Arr *arr);

#endif

