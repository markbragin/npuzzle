#ifndef _HEURISTICS_H_
#define _HEURISTICS_H_

#include <bits/stdint-uintn.h>

#include "types.h"

unsigned manhattan_dist(const Board board, unsigned size);
unsigned hamming_dist(const Board board, unsigned size);
unsigned linear_conflicts(const Board board, unsigned size);

#endif
