/*
 * Min-heap with int priorities and char* values.
 * It doesn't free memory allocated for values. This work is on the caller.
 */


#ifndef _MIN_HEAP_H_
#define _MIN_HEAP_H_

typedef struct {
    int priority;
    char *value;
} MinHeapItem;

typedef struct {
    MinHeapItem *items;
    unsigned size;
    unsigned capacity;
} MinHeap;

/* Creates min-heap allocating memory for *capacity* elements on a heap.
 * At least for 32 elements. */
MinHeap mh_create(unsigned capacity);

/* Destroys min-heap. Deleting values is on the caller */
void mh_destroy(MinHeap *mh);

/* Pushes value to the heap with given priority */
void mh_push(MinHeap *mh, int priority, char* value);

/* Returns the element on top of the heap (with lowest priority)
 * and deletes it. If size = 0 returns garbage */
MinHeapItem mh_pop(MinHeap *mh);

#endif
