#include "input.h"

#include <bits/types/FILE.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "heuristics.h"
#include "pattern_database.h"


void parse_args(int argc, char** argv, Heuristics *heuristics, Algo *algo)
{
    int i;

    if (argc < 2) {
        usage();
        exit(1);
    }

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-g") == 0 || strcmp(argv[i], "--greedy") == 0)
            *algo = GREEDY_BEFS;
        else if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--astar") == 0)
            *algo = ASTAR;
        else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--idastar") == 0)
            *algo = IDASTAR;
        else if (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--manhattan") == 0)
            *heuristics = manhattan_dist;
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--hamming") == 0)
            *heuristics = hamming_dist;
        else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--linear") == 0)
            *heuristics = linear_conflicts;
        else if (strcmp(argv[i], "-5") == 0 || strcmp(argv[i], "--database555") == 0)
            *heuristics = pattern_database555;
        else if (strcmp(argv[i], "-6") == 0 || strcmp(argv[i], "--database663") == 0)
            *heuristics = pattern_database663;
        else if (strcmp(argv[i], "--create555") == 0) {
            create_pattern_database555();
            exit(0);
        }
        else if (strcmp(argv[i], "--create663") == 0) {
            exit(0);
            create_pattern_database663();
        }
        else if (strcmp(argv[i], "--help") == 0) {
            usage();
            exit(0);
        }
    }
}

/* Returns:
 *     -1 if can't open file
 *     -2 if occured error while reading intergers from file
 *     -3 if > 256 integers
 */
int read_from_file(const char *filename, int *buffer)
{
    FILE *file;
    unsigned i;
    int status;

    file = fopen(filename, "r");
    if (!file)
        return -1;

    for (i = 0; i < 256; i++) {
        status = fscanf(file, "%d", &buffer[i]);
        if (status == 0) {
            fclose(file);
            return -2;
        }
        if (status == EOF)
            break;
    }
    fclose(file);
    if (status != EOF)
        return -3;
    return i;
}

/* Returns:
 *     0 if board is OK.
 *     -1 if board is not square
 *     -2 if numbers are not in [0; len - 1]
 *     -3 if numbers repeat themselves
 */
int validate_input(const int *buffer, unsigned len)
{
    unsigned size, i;
    unsigned char used[256];

    size = (unsigned)sqrt(len);
    if (size * size != len)
        return -1;
    
    memset(used, 0, 256);

    for (i = 0; i < len; i++) {
        if (buffer[i] < 0 || buffer[i] >= len)
            return -2;
        else if (used[buffer[i]])
            return -3;
        else
            used[buffer[i]] = 1;
    }
    return 0;
}


void usage(void)
{
    printf("USAGE:\n"
           "    npuzzle FILE [options]\n\n"
           "    Default options are --greedy --linear\n\n"
           "ARGS:\n"
           "    FILE\n"
           "        File with input board.\n\n"
           "OPTIONS:\n"
           "    -g, --greedy\n"
           "        Use greedy best first search algorithm.\n\n"
           "    -a, --astar\n"
           "        Use A* algorithm.\n\n"
           "    -i, --idastar\n"
           "        Use IDA* algorithm.\n\n");
    printf("    -h, --hamming\n"
           "        Use hamming distance heuristics.\n\n"
           "    -m, --manhattan\n"
           "        Use manhattan distance heuristics.\n\n"
           "    -l, --linear\n"
           "        Use linear conflicts heuristics.\n\n"
           "    -5, --database555\n"
           "        Use pattern database 5-5-5 heuristics.\n\n"
           "    -6, --database663\n"
           "        Use pattern database 6-6-5 heuristics.\n\n"
           "        --create555\n"
           "        Create pattern database 5-5-5.\n\n"
           "        --create663\n"
           "        Create pattern database 6-6-3.\n\n"
           "        --help\n"
           "        Print this help message.\n\n");
}
