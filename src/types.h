#ifndef _TYPES_H_
#define _TYPES_H_

#include <bits/stdint-uintn.h>

/* Board type */
typedef uint8_t* Board;

/* Tile type */
typedef uint8_t Tile;

/* Pointer to heuristic function */
typedef unsigned (*Heuristics)(const Board board, unsigned size);

/* Type of pathfinding algorithm */
typedef enum {
    GREEDY_BEFS,
    ASTAR,
    IDASTAR
} Algo;


#endif
