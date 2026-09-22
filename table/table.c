/*
 * table - Hash Table implementation
 *
 * - FNV-1a hash function algorithm.
 * - Open addressing for collisions
 *
 * References:
 * - Crafting Interpreters by Robert Nystrom (Chapter 20)
 */

#include <stdlib.h>
#include <string.h>

#include "table.h"

#define TABLE_MAX_LOAD 0.75
#define TABLE_MAX_CAPACITY ((size_t)-3)
#define TABLE_GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity) * 2)

// Reserved key_id values used as entry state indicators.
#define ENTRY_EMPTY     ((size_t)-1)
#define ENTRY_TOMBSTONE ((size_t)-2)

// FNV-1a hash function algorithm.
static uint32_t hash_string(const char *key, size_t len)
{
    uint32_t hash = 2166136261u;

    for (size_t i = 0; i < len; i++) {
        hash ^= (uint8_t)key[i];
        hash *= 16777619;
    }

    return hash;
}

void table_init(Table *table)
{
    table->entry_count = 0;
    table->entry_active_count = 0;
    table->key_count = 0;
    table->capacity = 0;
    table->free_key_id = ENTRY_EMPTY;
    table->entries = NULL;
    table->keys = NULL;
}

void table_free(Table *table)
{
    if (table->keys != NULL) {
        for (size_t i = 0; i < table->key_count; i++) {
            char *chars = table->keys[i].chars;
            if (chars == NULL) continue;
            free(chars);
            chars = NULL;
        }
        free(table->keys);
    }

    if (table->entries != NULL) free(table->entries);
    table_init(table);
}

static bool match_keys(Key *key1, Key *key2)
{
    return key1->len == key2->len &&
           key1->hash == key2->hash &&
           memcmp(key1->chars, key2->chars, key1->len) == 0;
}

static Entry *find_entry(Entry *entries, Key *keys, Key key, size_t capacity)
{
    uint32_t index = key.hash & (capacity - 1);
    Entry *tombstone = NULL;

    for (;;) {
        Entry *entry = &entries[index];

        if (entry->key_id == ENTRY_EMPTY) {
            return tombstone != NULL ? tombstone : entry;
        } else if (entry->key_id == ENTRY_TOMBSTONE) {
            if (tombstone == NULL) tombstone = entry;
        } else {
            Key key2 = keys[entry->key_id];
            if (match_keys(&key, &key2)) return entry;
        }

        index = (index + 1) & (capacity - 1);
    }
}

static bool adjust_capacity(Table *table, size_t capacity)
{
    if (capacity > TABLE_MAX_CAPACITY) return false;

    Entry *entries = malloc(sizeof(Entry) * capacity);
    if (entries == NULL) return false;

    Key *keys = malloc(sizeof(Key) * capacity);
    if (keys == NULL) {
        free(entries);
        return false;
    }

    for (size_t i = 0; i < capacity; i++) entries[i].key_id = ENTRY_EMPTY;

    size_t new_count = 0;
    for (size_t i = 0; i < table->capacity; i++) {
        Entry *entry = &table->entries[i];
        // Copy only non-empty/non-tombstone entries.
        if (entry->key_id == ENTRY_EMPTY ||
            entry->key_id == ENTRY_TOMBSTONE) continue;

        // Compact keys array into 0..new_count slots.
        keys[new_count] = table->keys[entry->key_id];

        Entry *dest = find_entry(entries, keys, keys[new_count], capacity);

        dest->key_id = new_count;
        dest->value = entry->value;

        new_count++;
    }

    if (table->entries != NULL) free(table->entries);
    if (table->keys != NULL) free(table->keys);

    table->entries = entries;
    table->keys = keys;
    table->entry_count = new_count;
    table->entry_active_count = new_count;
    table->key_count = new_count;
    table->capacity = capacity;
    table->free_key_id = ENTRY_EMPTY;

    return true;
}

static bool make_key(Key *key, const char *chars, size_t len, uint32_t hash)
{
    key->chars = malloc(sizeof(char) * (len + 1));
    if (key->chars == NULL) return false;

    memcpy(key->chars, chars, len);
    key->chars[len] = '\0';
    key->next_id = ENTRY_EMPTY;
    key->len = len;
    key->hash = hash;

    return true;
}

bool table_insert(Table *table, const char *key_chars, double value)
{
    if (table->capacity * TABLE_MAX_LOAD < table->entry_count + 1) {
        size_t capacity = TABLE_GROW_CAPACITY(table->capacity);
        if (!adjust_capacity(table, capacity)) return false;
    }
    
    size_t len = strlen(key_chars);
    uint32_t hash = hash_string(key_chars, len);
    Key key = { .len = len, .hash = hash, .chars = (char *)key_chars, };
    Entry *entry = find_entry
        (table->entries, table->keys, key, table->capacity);

    if (entry->key_id == ENTRY_EMPTY || entry->key_id == ENTRY_TOMBSTONE) {
        if (entry->key_id == ENTRY_EMPTY) table->entry_count++;

        // Check for previously deleted key slot first
        size_t key_id = table->free_key_id;
        if (key_id != ENTRY_EMPTY) {
            // Update head of free_key_id list
            table->free_key_id = table->keys[key_id].next_id;
        } else key_id = table->key_count++;

        Key *new_key = &table->keys[key_id];
        if (!make_key(new_key, key_chars, len, hash)) return false;
        entry->key_id = key_id;

        table->entry_active_count++;
    }

    entry->value = value;

    return true;
}

static Key temp_key(const char *chars)
{
    size_t len = strlen(chars);
    uint32_t hash = hash_string(chars, len);
    return (Key){ .len = len, .hash = hash, .chars = (char *)chars, };
}

bool table_get(Table *table, const char *key_chars, double *value)
{
    if (table->entry_count == 0) return false;

    Entry *entry = find_entry
        (table->entries, table->keys, temp_key(key_chars), table->capacity);

    if (entry->key_id == ENTRY_EMPTY ||
        entry->key_id == ENTRY_TOMBSTONE) return false;

    *value = entry->value;

    return true;
}

bool table_find(Table *table, const char *key_chars)
{
    if (table->entry_count == 0) return false;

    Entry *entry = find_entry
        (table->entries, table->keys, temp_key(key_chars), table->capacity);

    if (entry->key_id == ENTRY_EMPTY ||
        entry->key_id == ENTRY_TOMBSTONE) return false;

    return true;
}

bool table_erase(Table *table, const char *key_chars)
{
    if (table->entry_count == 0) return false;

    Entry *entry = find_entry
        (table->entries, table->keys, temp_key(key_chars), table->capacity);

    if (entry->key_id == ENTRY_EMPTY ||
        entry->key_id == ENTRY_TOMBSTONE) return false;

    Key *key = &table->keys[entry->key_id];
    if (key->chars != NULL) {
        free(key->chars);
        key->chars = NULL;
    }

    // Add key_id to free_key_id list.
    key->next_id = table->free_key_id;
    table->free_key_id = entry->key_id;
    entry->key_id = ENTRY_TOMBSTONE;

    table->entry_active_count--;
    return true;
}

size_t table_count(Table *table)
{
    return table->entry_active_count;
}

