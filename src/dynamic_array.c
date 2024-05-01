#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynamic_array.h"
#include "types.h"

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
    if (da.items == NULL) {
        fprintf(stderr, "Can't allocate memory\n");
        exit(6);
    }
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
        if (da->items[da->size] == NULL) {
            fprintf(stderr, "Can't allocate memory\n");
            exit(6);
        }
        da->nallocated++;
    }
    memcpy(da->items[da->size], value, da->item_len);
    da->size++;
}

/* Doubles the array size */
static void resize(DynamicArray *da)
{
    unsigned new_capacity;
    int i;

    i = 1;
    do {
        new_capacity = da->capacity * (1 + 1. / i);
        da->items = realloc(da->items, new_capacity * sizeof(Board));
        i++;
    } while (da->items == NULL && i <= 4);

    da->capacity = new_capacity;
    if (da->items == NULL) {
        fprintf(stderr, "Can't allocate memory\n");
        exit(6);
    }
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

void da_reverse(DynamicArray *da)
{
    unsigned i, n;
    Board tmp;

    for (i = 0, n = da->size; i < n / 2; i++) {
        tmp = da->items[i];
        da->items[i] = da->items[n - i - 1];
        da->items[n - i - 1] = tmp;
    }
}
