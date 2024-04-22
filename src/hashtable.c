/*
 * Simple char*:char* hashtable. It doesn't free memory allocated for keys and
 * values. This work is on the caller.
 */

#include "hashtable.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static unsigned MIN_CAPACITY = 32;

static void resize(Hashtable *ht);
static void insert(HashtableItem *items, unsigned capacity, char *key,
                   char *value);

unsigned hash(const char *str)
{
    /* MurmurOAAT_32 */
    unsigned h;

    h = 3323198485ul;
    for (; *str; ++str) {
        h ^= *str;
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }
    return h;
}

Hashtable ht_create(unsigned capacity)
{
    Hashtable ht;
    unsigned nbytes;

    ht.capacity = capacity < MIN_CAPACITY ? MIN_CAPACITY : capacity;
    nbytes = sizeof(HashtableItem) * ht.capacity;
    ht.size = 0;
    ht.items = malloc(nbytes);
    memset(ht.items, 0, nbytes);

    return ht;
}

void ht_destroy(Hashtable *ht)
{
    free(ht->items);
    ht->size = 0;
    ht->capacity = 0;
}

void ht_insert(Hashtable *ht, char *key, char *value)
{
    HashtableItem *item;

    if ((item = ht_find(ht, key))) {
        item->value = value;
    } else {
        if ((double)ht->size / ht->capacity > 0.75)
            resize(ht);

        insert(ht->items, ht->capacity, key, value);
        ht->size++;
    }
}

void resize(Hashtable *ht)
{
    unsigned i, count, new_capacity;
    HashtableItem *new_items, item;

    new_capacity = ht->capacity * 2;
    new_items = malloc(sizeof(HashtableItem) * new_capacity);
    for (i = 0, count = 0; i < ht->capacity && count < ht->size; i++) {
        item = ht->items[i];
        if (item.state == OCCUPIED)
            insert(new_items, new_capacity, item.key, item.value);
    }
    free(ht->items);
    ht->items = new_items;
    ht->capacity = new_capacity;
}

static void insert(HashtableItem *items, unsigned capacity, char *key,
                   char *value)
{
    HashtableItem new_item;
    unsigned idx;

    idx = hash(key) % capacity;
    while (items[idx].state != FREE)
        idx = (idx + 1) % capacity;

    new_item.key = key;
    new_item.value = value;
    new_item.state = OCCUPIED;
    items[idx] = new_item;
}

HashtableItem *ht_find(Hashtable *ht, const char *key)
{
    unsigned idx;

    idx = hash(key) % ht->capacity;
    while (ht->items[idx].state != FREE) {
        if (strcmp(ht->items[idx].key, key) == 0)
            return &ht->items[idx];
        idx = (idx + 1) % ht->capacity;
    }
    return NULL;
}

void ht_print(Hashtable *ht)
{
    HashtableItem item;
    unsigned i, count;
    for (i = 0, count = 0; i < ht->capacity && count < ht->size; i++) {
        item = ht->items[i];
        if (item.state == OCCUPIED) {
            printf("[%d] %s : %s\n", count, item.key, item.value);
            count++;
        }
    }
}
