/*
 * Simple dynamic array of char* which allocates memory for each element
 * and frees it on destroy.
 */


#ifndef _DYNAMIC_ARRAY_H_
#define _DYNAMIC_ARRAY_H_

typedef struct {
    char **items;
    unsigned size;
    unsigned capacity;
} DynamicArray;

/* Creates dynamic array allocation memory on a heap. At least for 32 elements */
DynamicArray da_create(unsigned capacity);

/* Frees all the memory allocated for every single item when destroing.
 * Don't use after destruction, create new one. */
void da_destroy(DynamicArray *da);

/* Pushes copy of value allocated on a heap to the back of the array */
void da_push(DynamicArray *da, const char *value);

/* Prints all the 'strings' in the array */
void da_print(DynamicArray *da);

#endif
