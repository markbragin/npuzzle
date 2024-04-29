#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "dynamic_array.h"
#include "hashtable.h"
#include "min_heap.h"
#include "pattern_database.h"
#include "types.h"

const char *DB_PATH555 = "database/pdb5-5-5";
const char *DB_PATH663 = "database/pdb6-6-3";

const Tile GROUP555_1[DB_BOARD_LEN]
    = { 1, 2, 3, 0, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
const Tile GROUP555_2[DB_BOARD_LEN]
    = { 0, 0, 0, 4, 0, 0, 7, 8, 0, 0, 11, 12, 0, 0, 0, 0 };
const Tile GROUP555_3[DB_BOARD_LEN]
    = { 0, 0, 0, 0, 0, 0, 0, 0, 9, 10, 0, 0, 13, 14, 15, 0 };

const Tile GROUP663_1[DB_BOARD_LEN]
    = { 1, 0, 0, 0, 5, 6, 0, 0, 9, 10, 0, 0, 13, 0, 0, 0 };
const Tile GROUP663_2[DB_BOARD_LEN]
    = { 0, 0, 0, 0, 0, 0, 7, 8, 0, 0, 11, 12, 0, 14, 15, 0 };
const Tile GROUP663_3[DB_BOARD_LEN]
    = { 0, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

static HashtableI PATTERN_DATABASE;
static DynamicArray BACKING_ARRAY;

static void generate_permutations(const Tile *group);
static void write_db_to_file(HashtableI *database, FILE *file);

void create_pattern_database555(void)
{
    FILE *file;
    clock_t start, end;

    system("mkdir database");
    file = fopen(DB_PATH555, "rb");
    if (file) {
        printf("File %s already exists.\nDo you want to rewrite it? [y/n]\n",
               DB_PATH555);
        fclose(file);
        if (getchar() != 'y')
            exit(0);
    }

    file = fopen(DB_PATH555, "wb");

    if (!file) {
        fprintf(stderr, "Can't write to file %s\n", DB_PATH555);
        exit(4);
    }

    PATTERN_DATABASE = hti_create(0, DB_BOARD_LEN);
    BACKING_ARRAY = da_create(0, DB_BOARD_LEN);

    printf("Generating database for 1 group\n");
    start = clock();
    generate_permutations(GROUP555_1);
    end = clock();
    printf("Time elapsed: %f\n", (double)(end - start) / CLOCKS_PER_SEC);

    write_db_to_file(&PATTERN_DATABASE, file);
    db_destroy();

    PATTERN_DATABASE = hti_create(0, DB_BOARD_LEN);
    BACKING_ARRAY = da_create(0, DB_BOARD_LEN);

    printf("Generating database for 2 group\n");
    start = clock();
    generate_permutations(GROUP555_2);
    end = clock();
    printf("Time elapsed: %f\n", (double)(end - start) / CLOCKS_PER_SEC);

    write_db_to_file(&PATTERN_DATABASE, file);
    db_destroy();

    PATTERN_DATABASE = hti_create(0, DB_BOARD_LEN);
    BACKING_ARRAY = da_create(0, DB_BOARD_LEN);

    printf("Generating database for 3 group\n");
    start = clock();
    generate_permutations(GROUP555_3);
    end = clock();
    printf("Time elapsed: %f\n", (double)(end - start) / CLOCKS_PER_SEC);

    write_db_to_file(&PATTERN_DATABASE, file);
    db_destroy();

    fclose(file);
}

static void write_db_to_file(HashtableI *database, FILE *file)
{
    unsigned i;
    HashtableIItem item;

    for (i = 0; i < PATTERN_DATABASE.capacity; i++) {
        item = PATTERN_DATABASE.items[i];
        if (item.state == OCCUPIED) {
            fwrite(item.key, 1, DB_BOARD_LEN, file);
            fputc(item.value, file);
        }
    }
}

void create_pattern_database663(void)
{
    FILE *file;
    clock_t start, end;

    system("mkdir database");
    file = fopen(DB_PATH663, "rb");
    if (file) {
        printf("File %s already exists.\nDo you want to rewrite it? [y/n]\n",
               DB_PATH663);
        fclose(file);
        if (getchar() != 'y')
            exit(0);
    }

    file = fopen(DB_PATH663, "wb");

    if (!file) {
        fprintf(stderr, "Can't write to file %s\n", DB_PATH555);
        exit(4);
    }

    PATTERN_DATABASE = hti_create(0, DB_BOARD_LEN);
    BACKING_ARRAY = da_create(0, DB_BOARD_LEN);

    printf("Generating database for 1 group\n");
    start = clock();
    generate_permutations(GROUP663_1);
    end = clock();
    printf("Time elapsed: %f\n", (double)(end - start) / CLOCKS_PER_SEC);

    write_db_to_file(&PATTERN_DATABASE, file);
    db_destroy();

    PATTERN_DATABASE = hti_create(0, DB_BOARD_LEN);
    BACKING_ARRAY = da_create(0, DB_BOARD_LEN);

    printf("Generating database for 2 group\n");
    start = clock();
    generate_permutations(GROUP663_2);
    end = clock();
    printf("Time elapsed: %f\n", (double)(end - start) / CLOCKS_PER_SEC);

    write_db_to_file(&PATTERN_DATABASE, file);
    db_destroy();

    PATTERN_DATABASE = hti_create(0, DB_BOARD_LEN);
    BACKING_ARRAY = da_create(0, DB_BOARD_LEN);

    printf("Generating database for 3 group\n");
    start = clock();
    generate_permutations(GROUP663_3);
    end = clock();
    printf("Time elapsed: %f\n", (double)(end - start) / CLOCKS_PER_SEC);

    write_db_to_file(&PATTERN_DATABASE, file);
    db_destroy();

    fclose(file);
}

static void generate_permutations(const Tile *group)
{
    int moves[4], new_empty, priority;
    unsigned i, checked_count, size, len;
    Tile cur_board[DB_BOARD_LEN];
    HashtableI visited;
    DynamicArray delete_later;
    MinHeap heap;
    MinHeapItem cur;

    len = DB_BOARD_LEN;
    size = DB_BOARD_SIZE;
    checked_count = 0;

    moves[0] = -size; /* up */
    moves[1] = 1;     /* right */
    moves[2] = size;  /* down */
    moves[3] = -1;    /* left */

    delete_later = da_create(1000 * 1000, len);
    visited = hti_create(1000 * 1000, len);
    heap = mh_create(1000 * 1000);

    memcpy(cur_board, group, len);

    da_push(&BACKING_ARRAY, cur_board);
    hti_insert(&PATTERN_DATABASE, da_back(&BACKING_ARRAY), 0);

    new_empty = len - 1;
    cur_board[len - 1] = 0xFF;
    da_push(&delete_later, cur_board);
    mh_push(&heap, 0, da_back(&delete_later), 0, new_empty);

    while (heap.size > 0) {
        checked_count++;
        cur = mh_pop(&heap);

        hti_insert(&visited, cur.board, cur.priority);

        for (i = 0; i < 4; i++) {
            if ((i == 0 && (cur.empty / size) == 0)
                || (i == 1 && (cur.empty % size) == size - 1)
                || (i == 2 && (cur.empty / size) == size - 1)
                || (i == 3 && (cur.empty % size) == 0))
                continue;

            new_empty = cur.empty + moves[i];
            memcpy(cur_board, cur.board, len);

            cur_board[cur.empty] = cur_board[new_empty];
            cur_board[new_empty] = 0xFF;

            if (hti_find(&visited, cur_board)) {
                continue;
            }

            if (cur_board[cur.empty] != 0) {
                priority = cur.priority + 1;
                cur_board[new_empty] = 0;
                da_push(&BACKING_ARRAY, cur_board);
                if (!hti_find(&PATTERN_DATABASE, cur_board))
                    hti_insert(&PATTERN_DATABASE, da_back(&BACKING_ARRAY),
                               priority);
                cur_board[new_empty] = 0xFF;
            } else
                priority = cur.priority;

            da_push(&delete_later, cur_board);
            mh_push(&heap, priority, da_back(&delete_later), priority,
                    new_empty);
        }
    }
    printf("Checked states: %u\n", checked_count);

    printf("Created %u records\n", PATTERN_DATABASE.size);
    da_destroy(&delete_later);
    hti_destroy(&visited);
    mh_destroy(&heap);
}

void read_pattern_database(const char *filename)
{
    Tile cur_board[DB_BOARD_LEN];
    int cost;
    FILE *file;

    file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Can't read database. Change path or create it\n");
        exit(4);
    }

    BACKING_ARRAY = da_create(3 * 1000 * 1000, DB_BOARD_LEN);
    PATTERN_DATABASE = hti_create(3 * 1000 * 1000, DB_BOARD_LEN);

    while (fread(cur_board, 1, DB_BOARD_LEN, file)) {
        cost = fgetc(file);
        da_push(&BACKING_ARRAY, cur_board);
        hti_insert(&PATTERN_DATABASE, da_back(&BACKING_ARRAY), cost);
    }

    printf("Database size: %u\n", PATTERN_DATABASE.size);
    fclose(file);
}

void db_destroy(void)
{
    hti_destroy(&PATTERN_DATABASE);
    da_destroy(&BACKING_ARRAY);
}

int db_get_cost(const Board board)
{
    HashtableIItem *item;
    item = hti_find(&PATTERN_DATABASE, board);
    if (item)
        return item->value;
    return -1;
}
