/*
 * table - Hash Table implementation
 *
 * - FNV-1a hash function algorithm.
 * - Open addressing for collisions
 *
 * References:
 * - Crafting Interpreters by Robert Nystrom (Chapter 20)
 */

#ifndef TABLE_H
#define TABLE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct Key {
    size_t next_id;
    size_t len;
    uint32_t hash;
    char *chars;
} Key;

typedef struct {
    size_t key_id;
    double value;
} Entry;

typedef struct {
    size_t entry_count;
    size_t entry_active_count;
    size_t key_count;
    size_t capacity;
    size_t free_key_id;
    Entry *entries;
    Key *keys;
} Table;

void table_init(Table *table);
void table_free(Table *table);

bool table_insert(Table *table, const char *key, double value);
bool table_get(Table *table, const char *key, double *value);
bool table_find(Table *table, const char *key);
bool table_erase(Table *table, const char *key);

size_t table_count(Table *table);

#endif

