/*
 * Min-heap with int priorities.
 * It doesn't free memory allocated for boards. This work is on the caller.
 */


#ifndef _MIN_HEAP_H_
#define _MIN_HEAP_H_

#include <bits/stdint-uintn.h>

#include "types.h"

typedef struct {
    int priority;
    Board board;
    unsigned depth;
    unsigned empty;
} MinHeapItem;

typedef struct {
    MinHeapItem *items;
    unsigned size;
    unsigned capacity;
} MinHeap;

/* Creates min-heap allocating memory for *capacity* items on a heap.
 * At least for 8 items. */
MinHeap mh_create(unsigned capacity);

/* Destroys min-heap. Deleting memory for boards is on the caller */
void mh_destroy(MinHeap *mh);

/* Pushes item to the heap with given priority */
void mh_push(MinHeap *mh, int priority, Board board, unsigned depth,
             unsigned empty);

/* Returns the item on top of the heap (with lowest priority)
 * and deletes it. If size = 0 returns garbage :( */
MinHeapItem mh_pop(MinHeap *mh);

#endif
