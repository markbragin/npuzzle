#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "dynamic_array.h"
#include "heuristics.h"
#include "input.h"
#include "pattern_database.h"
#include "solver.h"
#include "types.h"


int main(int argc, char **argv)
{
    unsigned size, i;
    int n, status, user_ans;
    clock_t start, end;

    int buffer[256];
    Tile board[256];

    DynamicArray ans;
    Heuristics heuristics;
    Algo algo;

    /* parse args */
    heuristics = linear_conflicts;
    algo = GREEDY_BEFS;
    parse_args(argc, argv, &heuristics, &algo);

    memset(buffer, 0, 256 * sizeof(int));

    /* reading from input file */
    n = read_from_file(argv[1], buffer);
    if (n == -1) {
        fprintf(stderr, "Error while opening file\n");
        return 2;
    } else if (n == -2) {
        fprintf(stderr, "Wrong format in input file\n");
        return 3;
    } else if (n == -3) {
        fprintf(stderr, "Max board size is 16x16\n");
        return 3;
    }

    /* validation of input */
    status = validate_input(buffer, n);
    if (status == -1) {
        fprintf(stderr, "Board is not square\n");
        return 3;
    } else if (status == -2) {
        fprintf(stderr, "Values must be in [0; number of elements - 1]\n");
        return 3;
    } else if (status == -3) {
        fprintf(stderr, "Values must not repeat themselves\n");
        return 3;
    }

    size = (unsigned)sqrt(n);
    buffer_to_board(buffer, board, n);

    printf("Input board:\n");
    print_board(board, size);

    /* check solvability */
    if (!is_solvable(board, size)) {
        printf("\nUnsolvable\n");
        return 3;
    }

    /* print info */
    putchar('\n');
    if (algo == ASTAR)
        printf("Algorithm: A*\n");
    else if (algo == GREEDY_BEFS)
        printf("Algorithm: Greedy Best First Search\n");
    else if (algo == IDASTAR)
        printf("Algorithm: IDA*\n");

    if (heuristics == manhattan_dist)
        printf("Heuristics: Manhattan distance\n");
    else if (heuristics == hamming_dist)
        printf("Heuristics: Hamming distance\n");
    else if (heuristics == linear_conflicts)
        printf("Heuristics: Linear conflicts\n");
    else if (heuristics == pattern_database555) {
        printf("Heuristics: Pattern database 5-5-5\n");
        printf("Reading database...\n");
        read_pattern_database(DB_PATH555);
    }
    else if (heuristics == pattern_database663) {
        printf("Heuristics: Pattern database 6-6-3\n");
        printf("Reading database...\n");
        read_pattern_database(DB_PATH663);
    }
    putchar('\n');

    /* run algo */
    start = clock();
    if (algo == IDASTAR)
        ans = idastar(board, size, heuristics);
    else
        ans = befs(board, size, algo, heuristics);
    end = clock();
    printf("\nTime elapsed: %.3fs\n", ((float)(end - start)) / CLOCKS_PER_SEC);

    if (heuristics == pattern_database555 || heuristics == pattern_database663)
        db_destroy();

    /* Print ans?? */
    printf("\nPrint answer? [y/n]\n");
    user_ans = getchar();
    if (user_ans == 'y') {
        for (i = 0; i < ans.size; i++) {
            printf("[%d]\n", i);
            print_board(ans.items[i], size);
            putchar('\n');
        }
    }

    da_destroy(&ans);

    return 0;
}
