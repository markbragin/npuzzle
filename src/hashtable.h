/*
 * Simple Board:Board (uint8_t:uint8_t) hashtable.
 * It doesn't free memory allocated for keys and values.
 * This work is on the caller.
 */

#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include <bits/stdint-uintn.h>

#include "types.h"


typedef enum {
    FREE,
    OCCUPIED,
    DELETED
} HashtableItemState;

typedef struct {
    Board key;
    Board value;
    HashtableItemState state;
} HashtableItem;

typedef struct {
   HashtableItem *items;
   unsigned size;
   unsigned capacity;
   unsigned key_len;
} Hashtable;

/* Calculates hash for uint8_t* array using MurmurOAAT_32 algo */
uint32_t hash(const uint8_t *str, unsigned len);

/* Creates hash table allocating memory for *capacity* items on a heap.
 * At least for 8 items. */
Hashtable ht_create(unsigned capacity, unsigned key_len);

/* Destroys hash table. Deleting keys and values is on the caller */
void ht_destroy(Hashtable *ht);

/* Inserts key value pair. If key exists replaces old value with new one*/
void ht_insert(Hashtable *ht, Board key, Board value);

/* Searches for key. If found one returns HashtableItem else return NULL */
HashtableItem *ht_find(Hashtable *ht, const Board key);

#endif
