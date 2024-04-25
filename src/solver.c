#include "solver.h"
#include "dynamic_array.h"
#include "hashtable.h"
#include "min_heap.h"
#include "types.h"

#include <bits/stdint-uintn.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static unsigned count_inversions(const Board board, unsigned size);
static int find_empty_tile(const Board board, unsigned size);
static DynamicArray generate_solution_array(Hashtable *visited, unsigned size);
static int idastar_search(DynamicArray *path, unsigned size, int empty,
                          int bound, Heuristics heuristics,
                          unsigned *checked_count);

void buffer_to_board(const int *buffer, Board board, unsigned len)
{
    unsigned i;

    for (i = 0; i < len; i++)
        board[i] = buffer[i];
}

void print_board(const Board board, unsigned size)
{
    unsigned i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++)
            printf("%3u", board[i * size + j]);
        printf("\n");
    }
}

int is_solvable(const Board board, unsigned size)
{
    unsigned inversions, i, row_num;

    inversions = count_inversions(board, size);
    if (size % 2 == 0) {
        for (i = 0; i < size * size; i++)
            if (board[i] == 0) {
                row_num = i / size;
                break;
            }
        if ((inversions % 2 == 0 && row_num % 2 != 0) ||
            (inversions % 2 != 0 && row_num % 2 == 0))
            return 1;
    } else {
        if (inversions % 2 == 0)
            return 1;
    }
    return 0;
}

static unsigned count_inversions(const Board board, unsigned size)
{
    unsigned inversions, i, j;

    inversions = 0;
    for (i = 0; i < size * size; i++) {
        if (board[i] <= 1)
            continue;
        for (j = i + 1; j < size * size; j++) {
            if (board[j] > 0 && board[j] < board[i])
                inversions++;
        }
    }
    return inversions;
}

static int find_empty_tile(const Board board, unsigned size)
{
    unsigned i;

    for (i = 0; i < size * size; i++)
        if (board[i] == 0)
            return i;
    return -1;
}

int is_solved(const Board board, unsigned size)
{
    unsigned i;
    for (i = 0; i < size * size - 1; i++)
        if (board[i] != i + 1)
            return 0;
    return 1;
}

void generate_goal(Board board, unsigned size)
{
    unsigned i;

    for (i = 0; i < size * size - 1; i++)
        board[i] = i + 1;
    board[size * size - 1] = 0;
}

/* If no solution returns empty array.
 * Caller must destroy returned array via da_destroy */
static DynamicArray generate_solution_array(Hashtable *visited, unsigned size)
{
    Tile goal[256];
    HashtableItem *item;
    DynamicArray ans;

    ans = da_create(0, size * size);

    generate_goal(goal, size);
    item = ht_find(visited, goal);
    da_push(&ans, item->key);
    while (item->value) {
        da_push(&ans, item->value);
        item = ht_find(visited, item->value);
    }
    da_reverse(&ans);
    return ans;
}

DynamicArray befs(Board board, unsigned size, Algo algo, Heuristics heuristics)
{
    Tile cur_board[256];
    unsigned checked_count, len, i;
    int empty, priority, moves[4];

    MinHeap heap;
    MinHeapItem cur;
    Hashtable visited;
    DynamicArray delete_later, ans;

    len = size * size;
    empty = find_empty_tile(board, size);
    if (empty < 0) {
        fprintf(stderr, "[BeFS]: no empty tile in the board\n");
        return da_create(1, size);
    }

    moves[0] = -size; /* up */
    moves[1] = 1;     /* right */
    moves[2] = size;  /* down */
    moves[3] = -1;    /* left */

    checked_count = 0;

    visited = ht_create(0, len);
    heap = mh_create(0);
    delete_later = da_create(0, len);

    da_push(&delete_later, board);
    mh_push(&heap, 0, da_back(&delete_later), 0, empty);
    ht_insert(&visited, da_back(&delete_later), NULL);

    while (heap.size > 0) {
        checked_count++;
        cur = mh_pop(&heap);
        if (is_solved(cur.board, size)) {
            printf("Found path with: %u steps\n", cur.depth);
            break;
        }

        for (i = 0; i < 4; i++) {
            if ((i == 0 && (cur.empty / size) == 0) ||
                (i == 1 && (cur.empty % size) == size - 1) ||
                (i == 2 && (cur.empty / size) == size - 1) ||
                (i == 3 && (cur.empty % size) == 0))
                continue;

            empty = cur.empty + moves[i];
            memcpy(cur_board, cur.board, len);

            cur_board[cur.empty] = cur_board[empty];
            cur_board[empty] = 0;

            if (ht_find(&visited, cur_board))
                continue;

            if (algo == ASTAR)
                priority = cur.depth + 1 + heuristics(cur_board, size);
            else
                priority = heuristics(cur_board, size);

            da_push(&delete_later, cur_board);
            ht_insert(&visited, da_back(&delete_later), cur.board);
            mh_push(&heap, priority, da_back(&delete_later), cur.depth + 1,
                    empty);
        }
    }

    ans = generate_solution_array(&visited, size);

    da_destroy(&delete_later);
    ht_destroy(&visited);
    mh_destroy(&heap);

    printf("Checked states: %u", checked_count);
    return ans;
}

DynamicArray idastar(Board board, unsigned size, Heuristics heuristics)
{
    int bound, new_bound, empty;
    unsigned checked_count;
    clock_t start, end;
    DynamicArray path;

    bound = heuristics(board, size);
    empty = find_empty_tile(board, size);

    path = da_create(100, size * size);
    da_push(&path, board);

    while (bound < INT_MAX) {
        checked_count = 0;
        start = clock();
        new_bound = idastar_search(&path, size, empty, bound, heuristics,
                                   &checked_count);
        end = clock();
        printf("Depth %3u. States checked: %10u. Time elapsed: %fs\n",
               bound, checked_count, (double)(end - start) / CLOCKS_PER_SEC);
        if (new_bound == -1)
            break;
        bound = new_bound;
    }
    printf("\nFound path with: %u steps\n", path.size - 1);
    printf("Checked states: %u", checked_count);
    return path;
}

/* Returns:
 *     -1: when solved;
 *     INT_MAX: when node has no successors */

static int idastar_search(DynamicArray *path, unsigned size, int empty,
                          int bound, Heuristics heuristics,
                          unsigned *checked_count)

{
    Tile cur_board[256];
    int estimated, min, new_empty, moves[4];
    unsigned depth, i;

    depth = path->size - 1;
    (*checked_count)++;

    estimated = depth + heuristics(da_back(path), size);
    if (estimated > bound)
        return estimated;
    if (is_solved(da_back(path), size))
        return -1;

    moves[0] = -size; /* up */
    moves[1] = 1;     /* right */
    moves[2] = size;  /* down */
    moves[3] = -1;    /* left */

    min = INT_MAX;
    for (i = 0; i < 4; i++) {
        if ((i == 0 && (empty / size) == 0) ||
            (i == 1 && (empty % size) == size - 1) ||
            (i == 2 && (empty / size) == size - 1) ||
            (i == 3 && (empty % size) == 0))
            continue;

        new_empty = empty + moves[i];
        memcpy(cur_board, da_back(path), size * size);

        cur_board[empty] = cur_board[new_empty];
        cur_board[new_empty] = 0;
        if (!da_contains(path, cur_board)) {
            da_push(path, cur_board);
            estimated = idastar_search(path, size, new_empty, bound, heuristics,
                                       checked_count);
            if (estimated == -1)
                return -1;
            if (estimated < min)
                min = estimated;
            da_pop(path);
        }
    }
    return min;
}
