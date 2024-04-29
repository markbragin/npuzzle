/*
 * Board:Board (uint8_t:uint8_t) hashtable.
 * Board:int hashtable
 * It doesn't free memory allocated for keys and values.
 * This work is on the caller.
 */

#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include <stdint.h>

#include "types.h"

/* Board:Board */

typedef enum {
    FREE,
    OCCUPIED,
    DELETED
} HashtableItemState;

typedef struct {
    Board key;
    Board value;
    HashtableItemState state;
} HashtableBItem;

typedef struct {
    HashtableBItem *items;
    unsigned size;
    unsigned capacity;
    unsigned key_len;
} HashtableB;

/* Calculates hash for uint8_t* array using MurmurOAAT_32 algo */
uint32_t hash(const uint8_t *str, unsigned len);

/* Creates hash table allocating memory for *capacity* items on a heap.
 * At least for 8 items. */
HashtableB htb_create(unsigned capacity, unsigned key_len);

/* Destroys hash table. Deleting keys and values is on the caller */
void htb_destroy(HashtableB *ht);

/* Inserts key value pair. If key exists replaces old value with new one*/
void htb_insert(HashtableB *ht, Board key, Board value);

/* Searches for key. If found one returns HashtableItem else return NULL */
HashtableBItem *htb_find(HashtableB *ht, const Board key);

/* Board:int */

typedef struct {
    Board key;
    int value;
    HashtableItemState state;
} HashtableIItem;

typedef struct {
    HashtableIItem *items;
    unsigned size;
    unsigned capacity;
    unsigned key_len;
} HashtableI;

/* Creates hash table allocating memory for *capacity* items on a heap.
 * At least for 8 items. */
HashtableI hti_create(unsigned capacity, unsigned key_len);

/* Destroys hash table. Deleting keys and values is on the caller */
void hti_destroy(HashtableI *ht);

/* Inserts key value pair. If key exists replaces old value with new one*/
void hti_insert(HashtableI *ht, Board key, int value);

/* Searches for key. If found one returns HashtableItem else return NULL */
HashtableIItem *hti_find(HashtableI *ht, const Board key);

#endif
