#include "dynamic_array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned MIN_CAPACITY = 32;

static void resize(DynamicArray *da);

DynamicArray da_create(unsigned capacity)
{
    unsigned nbytes;
    DynamicArray da;

    da.capacity = capacity < MIN_CAPACITY ? MIN_CAPACITY : capacity;
    nbytes = sizeof(char*) * capacity;
    da.size = 0;
    da.items = malloc(nbytes);
    return da;
}

void da_destroy(DynamicArray *da)
{
    unsigned i;
    for (i = 0; i < da->size; i++)
        free(da->items[i]);
    free(da->items);
}

void da_push(DynamicArray *da, const char *value)
{
    if (da->size == da->capacity)
        resize(da);
    da->items[da->size] = malloc((strlen(value) + 1) * sizeof(char));
    strcpy(da->items[da->size], value);
    da->size++;
}

/* Doubles the array size */
static void resize(DynamicArray *da)
{
    da->capacity = da->capacity * 2;
    da->items = realloc(da->items, da->capacity * sizeof(da->items[0]));
}

void da_print(DynamicArray *da)
{
    unsigned i;
    for (i = 0; i < da->size; i++)
        printf("[%d] %s\n", i, da->items[i]);
}
