#ifndef _INPUT_H_
#define _INPUT_H_

#include "types.h"

/* Parses arguments and sets heuristics and algorithm if any */
void parse_args(int argc, char** argv, Heuristics *heuristics, Algo *algo);

/* Reads board from file. Returns number of elements read */
int read_from_file(const char *filename, int *buffer);

/* Validates if input contains n^n elements, values in [0; n^n - 1]
 * and don't repeat themselves */
int validate_input(const int *buffer, unsigned len);

/* Translates int[] buffer to Tile[] board (*Board) */
void buffer_to_board(const int *buffer, Board board, unsigned len);

/* Prints usage */
void usage(void);

#endif
