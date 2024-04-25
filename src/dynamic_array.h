/*
 * Simple dynamic array of Boards (uint8_t*) (constant length)
 * which allocates memory for each element and frees it on destroy.
 */

#ifndef _DYNAMIC_ARRAY_H_
#define _DYNAMIC_ARRAY_H_

#include <bits/stdint-uintn.h>

#include "types.h"


typedef struct {
    Board *items;
    unsigned size;
    unsigned capacity;
    unsigned item_len;
    unsigned nallocated;
} DynamicArray;

/* Creates dynamic array allocation memory on a heap. At least for 8 items */
DynamicArray da_create(unsigned capacity, unsigned item_len);

/* Frees all the memory allocated for every single item when destroying.
 * Don't use after destruction, create new one */
void da_destroy(DynamicArray *da);

/* Pushes copy of value allocated on a heap to the back of the array */
void da_push(DynamicArray *da, const Board value);

/* Returns last item in the arrays */
Board da_back(DynamicArray *da);

/* Deletes last item without freein memory */
void da_pop(DynamicArray *da);

/* Check if the array contain item */
int da_contains(DynamicArray *da, Board board);

/* Reverses the array */
void da_reverse(DynamicArray *da);

#endif
