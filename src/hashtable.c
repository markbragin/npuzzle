#include "hashtable.h"

#include <bits/stdint-uintn.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static unsigned MIN_CAPACITY = 8;


/* HashtableB */

static void resizeb(HashtableB *ht);
static void insertb(HashtableBItem *items, unsigned capacity, Board key,
                   Board value, unsigned key_len);

uint32_t hash(const uint8_t *str, unsigned len)
{
    /* MurmurOAAT_32 */
    uint32_t h;
    unsigned i;

    h = 3323198485ul;
    for (i = 0; i < len; i++) {
        h ^= str[i];
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }
    return h;
}

HashtableB htb_create(unsigned capacity, unsigned key_len)
{
    HashtableB ht;
    unsigned nbytes;

    ht.capacity = capacity < MIN_CAPACITY ? MIN_CAPACITY : capacity;
    nbytes = sizeof(HashtableBItem) * ht.capacity;
    ht.size = 0;
    ht.items = malloc(nbytes);
    ht.key_len = key_len;
    memset(ht.items, 0, nbytes);

    return ht;
}

void htb_destroy(HashtableB *ht)
{
    free(ht->items);
    ht->size = 0;
    ht->capacity = 0;
}

void htb_insert(HashtableB *ht, Board key, Board value)
{
    HashtableBItem *item;

    if ((item = htb_find(ht, key))) {
        item->value = value;
    } else {
        if ((double)ht->size / ht->capacity > 0.75)
            resizeb(ht);

        insertb(ht->items, ht->capacity, key, value, ht->key_len);
        ht->size++;
    }
}

/* Double the backing array size. Used by ht_insert() function when load factor
 * exceedes 0.75 */
static void resizeb(HashtableB *ht)
{
    unsigned i, count, new_capacity, nbytes;
    HashtableBItem *new_items, item;

    new_capacity = ht->capacity * 2;
    nbytes = sizeof(HashtableBItem) * new_capacity;
    new_items = malloc(nbytes);
    memset(new_items, 0, nbytes);
    for (i = 0, count = 0; i < ht->capacity && count < ht->size; i++) {
        item = ht->items[i];
        if (item.state == OCCUPIED)
            insertb(new_items, new_capacity, item.key, item.value, ht->key_len);
    }
    free(ht->items);
    ht->items = new_items;
    ht->capacity = new_capacity;
}

/* Inserts key value pair into backing array */
static void insertb(HashtableBItem *items, unsigned capacity, Board key,
                   Board value, unsigned key_len)
{
    HashtableBItem new_item;
    unsigned idx;

    idx = hash(key, key_len) % capacity;
    while (items[idx].state != FREE)
        idx = (idx + 1) % capacity;

    new_item.key = key;
    new_item.value = value;
    new_item.state = OCCUPIED;
    items[idx] = new_item;
}

HashtableBItem *htb_find(HashtableB *ht, const Board key)
{
    unsigned idx;

    idx = hash(key, ht->key_len) % ht->capacity;
    while (ht->items[idx].state != FREE) {
        if (memcmp(ht->items[idx].key, key, ht->key_len) == 0)
            return &ht->items[idx];
        idx = (idx + 1) % ht->capacity;
    }
    return NULL;
}



/* HashtableI */

static void resizei(HashtableI *ht);
static void inserti(HashtableIItem *items, unsigned capacity, Board key,
                    int value, unsigned key_len);


HashtableI hti_create(unsigned capacity, unsigned key_len)
{
    HashtableI ht;
    unsigned nbytes;

    ht.capacity = capacity < MIN_CAPACITY ? MIN_CAPACITY : capacity;
    nbytes = sizeof(HashtableIItem) * ht.capacity;
    ht.size = 0;
    ht.items = malloc(nbytes);
    ht.key_len = key_len;
    memset(ht.items, 0, nbytes);

    return ht;
}

void hti_destroy(HashtableI *ht)
{
    free(ht->items);
    ht->size = 0;
    ht->capacity = 0;
}

void hti_insert(HashtableI *ht, Board key, int value)
{
    HashtableIItem *item;

    if ((item = hti_find(ht, key))) {
        item->value = value;
    } else {
        if ((double)ht->size / ht->capacity > 0.75)
            resizei(ht);

        inserti(ht->items, ht->capacity, key, value, ht->key_len);
        ht->size++;
    }
}

/* Double the backing array size. Used by ht_insert() function when load factor
 * exceedes 0.75 */
static void resizei(HashtableI *ht)
{
    unsigned i, count, new_capacity, nbytes;
    HashtableIItem *new_items, item;

    new_capacity = ht->capacity * 2;
    nbytes = sizeof(HashtableIItem) * new_capacity;
    new_items = malloc(nbytes);
    memset(new_items, 0, nbytes);
    for (i = 0, count = 0; i < ht->capacity && count < ht->size; i++) {
        item = ht->items[i];
        if (item.state == OCCUPIED)
            inserti(new_items, new_capacity, item.key, item.value, ht->key_len);
    }
    free(ht->items);
    ht->items = new_items;
    ht->capacity = new_capacity;
}

/* Inserts key value pair into backing array */
static void inserti(HashtableIItem *items, unsigned capacity, Board key,
                    int value, unsigned key_len)
{
    HashtableIItem new_item;
    unsigned idx;

    idx = hash(key, key_len) % capacity;
    while (items[idx].state != FREE)
        idx = (idx + 1) % capacity;

    new_item.key = key;
    new_item.value = value;
    new_item.state = OCCUPIED;
    items[idx] = new_item;
}

HashtableIItem *hti_find(HashtableI *ht, const Board key)
{
    unsigned idx;

    idx = hash(key, ht->key_len) % ht->capacity;
    while (ht->items[idx].state != FREE) {
        if (memcmp(ht->items[idx].key, key, ht->key_len) == 0)
            return &ht->items[idx];
        idx = (idx + 1) % ht->capacity;
    }
    return NULL;
}
