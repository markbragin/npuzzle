#ifndef _SOLVER_H_
#define _SOLVER_H_

#include <bits/stdint-uintn.h>

#include "dynamic_array.h"
#include "types.h"

/* Prints board */
void print_board(const Board board, unsigned size);

/* Check if the game is solvable. Check if board is valid before calling */
int is_solvable(const Board board, unsigned size);

/* Check if current board is solved */ 
int is_solved(const Board board, unsigned size);

/* Generates goal for given size */
void generate_goal(Board board, unsigned size);

/* Solves npuzzle using greedy best first search or A*.
 * Caller must destroy returned array via da_destroy */
DynamicArray befs(Board board, unsigned size, Algo befs_type,
                  Heuristics heuristic);

/* Solves npuzzle using IDA*.
 * Caller must destroy returned array via da_destroy */
DynamicArray idastar(Board board, unsigned size, Heuristics heuristic);

#endif
