/* arr - Dynamic array
 *
 * Arr struct is created with a starting `capacity`. When the array is full,
 * it will automatically resize by doubling its current capacity.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    size_t size;
    size_t capacity;
    size_t elem_size;
    void *data;    
} Arr;

Arr *arr_create(size_t elem_size, size_t capacity)
{
    if (capacity == 0) {
        return NULL;
    }
    Arr *arr;
    if ((arr = malloc(sizeof(Arr))) == NULL) {
        return NULL;
    }
    if ((arr->data = malloc(capacity*elem_size)) == NULL) {
        free(arr);
        return NULL;
    }
    arr->elem_size = elem_size;
    arr->capacity = capacity;
    arr->size = 0;
    return arr;    
}

void arr_free(Arr *arr)
{
    if (arr == NULL) {
        return;
    }
    free(arr->data);
    free(arr);
}

static bool arr_realloc(Arr *arr, size_t capacity)
{
    void *temp = realloc(arr->data, capacity * arr->elem_size);
    if (temp == NULL) {
        return false;
    }
    arr->data = temp;
    arr->capacity = capacity;
    return true;
}

void arr_clear(Arr *arr)
{
    arr->size = 0;
}

void *arr_reserve(Arr *arr, size_t capacity)
{
    if (capacity > arr->capacity) {
        if (!arr_realloc(arr, capacity)) {
            return NULL;
        }
    }
    return (uint8_t *)arr->data;
}

void *arr_push(Arr *arr, void *elem)
{
    if (arr->size >= arr->capacity) {
        if (!arr_realloc(arr, arr->capacity*2)) {
            return NULL;
        }
    }
    uint8_t *base = arr->data;
    size_t offset_next = arr->size * arr->elem_size;
    memcpy(base + offset_next, elem, arr->elem_size);
    arr->size++;
    return (uint8_t *)base + offset_next;
}

void *arr_pop(Arr *arr)
{
    if (arr->size == 0) {
        return NULL;
    }
    arr->size--;
    size_t offset_pop = arr->size * arr->elem_size;
    return (uint8_t *)arr->data + offset_pop;
}

void *arr_get(Arr *arr, size_t index)
{
    if (index >= arr->size) {
        return NULL;
    }
    size_t offset_idx = (size_t)index * arr->elem_size;
    return (uint8_t *)arr->data + offset_idx;
}

size_t arr_size(Arr *arr)
{
    return arr->size;
}

