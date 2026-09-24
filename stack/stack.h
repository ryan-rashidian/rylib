/*
 * Stack - LIFO
 */

#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include <stddef.h>

#ifndef STACK_VALUE_TYPE
#define STACK_VALUE_TYPE double
#endif

typedef STACK_VALUE_TYPE SValue;

typedef struct Stack Stack;

Stack *stack_init(void);
void stack_free(Stack *stack);

bool stack_push(Stack *stack, SValue value);
bool stack_pop(Stack *stack, SValue *value);

#define STACK_IMPLEMENTATION
#ifdef STACK_IMPLEMENTATION
#ifndef STACK_IMPL_GAURD
#define STACK_IMPL_GAURD

#include <stdlib.h>

#define STACK_GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity) * 2)

struct Stack {
    size_t count;
    size_t capacity;
    SValue *values;
};

Stack *stack_init(void)
{
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    if (stack == NULL) return NULL;

    stack->count = 0;
    stack->capacity = 0;
    stack->values = NULL;

    return stack;
}

void stack_free(Stack *stack)
{
    if (stack == NULL) return;

    if (stack->values != NULL) free(stack->values);
    free(stack);
}

static bool stack_grow(Stack *stack, size_t capacity)
{
    SValue *temp = (SValue *)realloc(stack->values, capacity);
    if (temp == NULL) return false;

    stack->values = temp;
    stack->capacity = capacity;

    return true;
}

bool stack_push(Stack *stack, SValue value)
{
    if (stack->capacity < stack->count + 1) {
        size_t new_capacity = STACK_GROW_CAPACITY(stack->capacity);
        if (!stack_grow(stack, new_capacity)) return false;
    }

    stack->values[stack->count++] = value;

    return true;
}

bool stack_pop(Stack *stack, SValue *value)
{
    if (stack->count == 0) return false;

    *value = stack->values[--stack->count];

    return true;
}

#endif
#endif
#endif

