#include "dynamic_array.h"

#include <bits/stdint-uintn.h>
#include <stdlib.h>
#include <string.h>

static unsigned MIN_CAPACITY = 8;

static void resize(DynamicArray *da);

DynamicArray da_create(unsigned capacity, unsigned item_len)
{
    unsigned nbytes;
    DynamicArray da;

    da.capacity = capacity < MIN_CAPACITY ? MIN_CAPACITY : capacity;
    nbytes = sizeof(Board) * da.capacity;
    da.size = 0;
    da.item_len = item_len;
    da.nallocated = 0;
    da.items = malloc(nbytes);
    return da;
}

void da_destroy(DynamicArray *da)
{
    unsigned i;
    for (i = 0; i < da->nallocated; i++)
        free(da->items[i]);
    free(da->items);
}

void da_push(DynamicArray *da, const Board value)
{
    if (da->size == da->capacity)
        resize(da);
    if (da->size >= da->nallocated) {
        da->items[da->size] = malloc(da->item_len * sizeof(Tile));
        da->nallocated++;
    }
    memcpy(da->items[da->size], value, da->item_len);
    da->size++;
}

/* Doubles the array size */
static void resize(DynamicArray *da)
{
    da->capacity = da->capacity * 2;
    da->items = realloc(da->items, da->capacity * sizeof(Board));
}

Board da_back(DynamicArray *da)
{
    return da->items[da->size - 1];
}

void da_pop(DynamicArray *da)
{
    if (da->size > 0)
        da->size--;
}

int da_contains(DynamicArray *da, Board board)
{
    unsigned i;
    for (i = 0; i < da->size; i++)
        if (memcmp(da->items[i], board, da->item_len) == 0)
            return 1;
    return 0;
}
