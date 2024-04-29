#ifndef _HEURISTICS_H_
#define _HEURISTICS_H_

#include "types.h"

unsigned manhattan_dist(const Board board, unsigned size);
unsigned hamming_dist(const Board board, unsigned size);
unsigned linear_conflicts(const Board board, unsigned size);
unsigned pattern_database555(const Board board, unsigned size);
unsigned pattern_database663(const Board board, unsigned size);

#endif
