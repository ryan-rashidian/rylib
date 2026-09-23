/*
 * list - Doubly linked list
 */

#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

#ifndef LIST_VALUE_TYPE
#define LIST_VALUE_TYPE double
#endif

typedef LIST_VALUE_TYPE LValue;

typedef struct Node* ListIter;
typedef struct List List;

List *list_init(void);
void list_free(List *list);

bool list_push_front(List *list, LValue value);
bool list_push_back(List *list, LValue value);
bool list_pop_front(List *list, LValue *value);
bool list_pop_back(List *list, LValue *value);

ListIter list_iter_begin(List *list);
ListIter list_iter_end(List *list);
void list_iter_next(ListIter *it);
bool list_iter_advance(ListIter *it, LValue dest);
bool list_insert(ListIter it, LValue value);
bool list_get(ListIter it, LValue *value);
bool list_erase(List *list, ListIter it);

#ifdef LIST_IMPLEMENTATION
#ifndef LIST_IMPL_GAURD
#define LIST_IMPL_GAURD

#include <stdlib.h>

typedef struct Node {
    struct Node *next;
    LValue value;
} Node;

struct List {
    Node *head;
    Node *tail;
};

List *list_init(void)
{
    List *list = (List *)malloc(sizeof(List));
    if (list == NULL) return NULL;

    list->head = NULL;
    list->tail = NULL;

    return list;
}

void list_free(List *list)
{
    if (list == NULL) return;

    Node *curr = list->head;
    while (curr) {
        Node *next = curr->next;
        free(curr);
        curr = next;
    }

    free(list);
}

static Node *make_node(LValue value)
{
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) return NULL;

    node->value = value;
    node->next = NULL;

    return node;
}

bool list_push_front(List *list, LValue value)
{
    Node *new_node = make_node(value);
    if (new_node == NULL) return false;

    new_node->next = list->head;
    list->head = new_node;

    if (list->tail == NULL) list->tail = new_node;

    return true;
}

bool list_push_back(List *list, LValue value)
{
    Node *new_node = make_node(value);
    if (new_node == NULL) return false;

    list->tail->next = new_node;
    list->tail = new_node;

    if (list->head == NULL) list->head = new_node;

    return true;
}

bool list_pop_front(List *list, LValue *value)
{
    if (list->head == NULL) return false;

    if (value != NULL) *value = list->head->value;

    Node *pop = list->head;
    list->head = pop->next;
    free(pop);

    return true;
}

bool list_pop_back(List *list, LValue *value)
{
    if (list->tail == NULL) return false;

    if (value != NULL) *value = list->tail->value;

    free(list->tail);
    list->tail = NULL;

    Node *new_tail = NULL;
    if (list->head != NULL) {
        Node *new_tail = list->head;
        while (new_tail->next != NULL) new_tail = new_tail->next;
    }
    list->tail = new_tail;

    return true;
}

ListIter list_iter_begin(List *list)
{
    return list->head;
}

ListIter list_iter_end(List *list)
{
    (void)list;
    return NULL;
}

void list_iter_next(ListIter *it)
{
    if (it == NULL) return;
    *it = (*it)->next;
}

bool list_iter_advance(ListIter *it, LValue dest)
{
    while (*it != NULL && (*it)->value != dest) *it = (*it)->next;

    if (it == NULL) return false;
    return true;
}

bool list_insert(ListIter it, LValue value)
{
    if (it == NULL) return false;

    it->value = value;

    return true;
}

bool list_get(ListIter it, LValue *value)
{
    if (it == NULL) return false;

    *value = it->value;

    return true;
}

bool list_erase(List *list, ListIter it)
{
    if (list->head == NULL || it == NULL) return false;

    if (it == list->head) {
        list_pop_front(list, NULL);
        return true;
    }
    if (it == list->tail) {
        list_pop_back(list, NULL);
        return true;
    }

    Node *curr = list->head->next;
    Node *prev = list->head;
    while(curr != NULL) {
        if (curr == it) {
            prev->next = curr->next;
            free(curr);
            return true;
        }
        prev = curr;
        curr = curr->next;
    }

    return false;
}

#endif
#endif
#endif

