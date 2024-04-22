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

unsigned hash(const char* str);
Hashtable ht_create(unsigned capacity);
void ht_destroy(Hashtable *ht);
void ht_insert(Hashtable *ht, char *key, char *value);
HashtableItem *ht_find(Hashtable *ht, const char *key);
void ht_print(Hashtable *ht);

#endif
