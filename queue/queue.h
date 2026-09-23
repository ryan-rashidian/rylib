/*
 * queue - FIFO
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

#ifndef QUEUE_VALUE_TYPE
#define QUEUE_VALUE_TYPE double
#endif

typedef QUEUE_VALUE_TYPE QValue;

typedef struct Queue Queue;

Queue *queue_init(void);
void queue_free(Queue *q);

bool queue_push(Queue *q, QValue value);
bool queue_pop(Queue *q, QValue *value);

#ifdef QUEUE_IMPLEMENTATION
#ifndef QUEUE_IMPL_GAURD
#define QUEUE_IMPL_GAURD

#include <stdlib.h>

#define QUEUE_GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity) * 2)

struct Queue {
    size_t capacity;
    size_t head, tail;
    QValue *buffer;
};

Queue *queue_init(void)
{
    Queue *q = malloc(sizeof(Queue));
    if (q == NULL) return NULL;

    q->capacity = 0;
    q->head = 0;
    q->tail = 0;
    q->buffer = NULL;

    return q;
}

void queue_free(Queue *q)
{
    if (q == NULL) return;

    if (q->buffer != NULL) free(q->buffer);
    free(q);
}

static bool grow_queue(Queue *q, size_t capacity)
{
    QValue *new_buffer = malloc(sizeof(QValue) * capacity);
    if (new_buffer == NULL) return false;

    size_t i = 0;
    for (size_t j = q->head; j != q->tail; j = (j + 1) % q->capacity) {
        new_buffer[i++] = q->buffer[j];
    }

    if (q->buffer != NULL) free(q->buffer);

    q->buffer = new_buffer;
    q->head = 0;
    q->tail = i;
    q->capacity = capacity;

    return true;
}

bool queue_push(Queue *q, QValue value)
{
    if (q->capacity == 0 || (q->tail + 1) % q->capacity == q->head) {
        size_t new_capacity = QUEUE_GROW_CAPACITY(q->capacity);
        if (!grow_queue(q, new_capacity)) return false;
    }

    q->buffer[q->tail] = value;
    q->tail = (q->tail + 1) % q->capacity;

    return true;
}

bool queue_pop(Queue *q, QValue *value)
{
    if (q->tail == q->head) return false;

    *value = q->buffer[q->head];
    q->head = (q->head + 1) % q->capacity;

    return true;
}

#endif
#endif
#endif

