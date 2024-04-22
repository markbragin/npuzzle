/*
 * Simple char*:char* hashtable. It doesn't free memory allocated for keys and
 * values. This work is on the caller.
 */


#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

typedef enum {
    FREE,
    OCCUPIED,
    DELETED
} HashtableItemState;

typedef struct {
    char *key;
    char *value;
    HashtableItemState state;
} HashtableItem;

typedef struct {
   HashtableItem *items;
   unsigned size;
   unsigned capacity;
} Hashtable;

/* Calculates hash for char* str using MurmurOAAT_32 algo */
unsigned hash(const char* str);

/* Creates hash table allocating memory for *capacity* elements on a heap.
 * At least for 32 elements. */
Hashtable ht_create(unsigned capacity);

/* Destroys hash table. Deleting keys and values is on the caller */
void ht_destroy(Hashtable *ht);

/* Inserts key value pair. If key exists replaces old value with new one*/
void ht_insert(Hashtable *ht, char *key, char *value);

/* Searches for key. If found one returns HashtableItem else return NULL */
HashtableItem *ht_find(Hashtable *ht, const char *key);

/* Prints all key values pairs to stdin */
void ht_print(Hashtable *ht);

#endif
