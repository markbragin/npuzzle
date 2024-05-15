#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "min_heap.h"

static unsigned MIN_CAPACITY = 8;

static void resize(MinHeap *mh);
static void sift_up(MinHeap *mh);
static void sift_down(MinHeap *mh);

static void swap(MinHeapItem *a, MinHeapItem *b)
{
    MinHeapItem tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

MinHeap mh_create(unsigned capacity)
{
    MinHeap mh;
    unsigned nbytes;

    mh.capacity = capacity < MIN_CAPACITY ? MIN_CAPACITY : capacity;
    nbytes = sizeof(MinHeapItem) * mh.capacity;
    mh.size = 0;
    mh.items = malloc(nbytes);
    if (mh.items == NULL) {
        fprintf(stderr, "Can't allocate memory\n");
        exit(6);
    }
    memset(mh.items, 0, nbytes);
    return mh;
}

void mh_destroy(MinHeap *mh)
{
    free(mh->items);
    mh->size = 0;
    mh->capacity = 0;
}

void mh_push(MinHeap *mh, int priority, Board board, unsigned depth,
             unsigned empty)
{
    MinHeapItem item;

    if (mh->size >= mh->capacity)
        resize(mh);

    item.priority = priority;
    item.board = board;
    item.depth = depth;
    item.empty = empty;
    mh->items[mh->size++] = item;
    sift_up(mh);
}

/* Double the backing array size */
static void resize(MinHeap *mh)
{
    MinHeapItem *new_items;
    unsigned new_capacity;
    int i;

    i = 1;
    do {
        new_capacity = mh->capacity * (1 + 1. / i);
        new_items = realloc(mh->items, new_capacity * sizeof(MinHeapItem));
        i++;
    } while (new_items == NULL && i <= 4);

    if (new_items == NULL) {
        fprintf(stderr, "Can't allocate memory\n");
        exit(6);
    }

    mh->items = new_items;
    mh->capacity = new_capacity;
}

/* Fixes heap property after insertion element into idx */
static void sift_up(MinHeap *mh)
{
    unsigned idx, par_idx;

    idx = mh->size - 1;
    par_idx = (idx - 1) / 2;

    while (idx > 0 && mh->items[idx].priority <= mh->items[par_idx].priority) {
        swap(&mh->items[idx], &mh->items[par_idx]);
        idx = par_idx;
        par_idx = (idx - 1) / 2;
    }
}

MinHeapItem mh_pop(MinHeap *mh)
{
    MinHeapItem item;
    item = mh->items[0];

    /* swap first element with last */
    swap(&mh->items[0], &mh->items[mh->size - 1]);
    mh->size--;
    sift_down(mh);

    return item;
}

static void sift_down(MinHeap *mh)
{
    unsigned min_idx, idx, l_idx, r_idx;

    idx = 0;
    for (;;) {
        min_idx = idx;
        l_idx = 2 * idx + 1;
        r_idx = 2 * idx + 2;

        if (l_idx < mh->size
            && mh->items[l_idx].priority <= mh->items[min_idx].priority)
            min_idx = l_idx;

        if (r_idx < mh->size
            && mh->items[r_idx].priority <= mh->items[min_idx].priority)
            min_idx = r_idx;

        if (min_idx == idx)
            break;

        swap(&mh->items[idx], &mh->items[min_idx]);
        idx = min_idx;
    }
}
