#ifndef _DYNAMIC_ARRAY_H_
#define _DYNAMIC_ARRAY_H_

typedef struct {
    char **items;
    unsigned size;
    unsigned capacity;
} DynamicArray;

DynamicArray da_create(unsigned capacity);
void da_destroy(DynamicArray *da);
void da_push(DynamicArray *da, const char *value);
void da_print(DynamicArray *da);

#endif
