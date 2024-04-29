#ifndef _PATTERN_DATABASE_H_
#define _PATTERN_DATABASE_H_

#include "types.h"

#define DB_BOARD_SIZE 4
#define DB_BOARD_LEN 16

/* Path to database */
extern const char *DB_PATH555;
extern const char *DB_PATH663;

/* database patterns */
extern const Tile GROUP555_1[DB_BOARD_LEN];
extern const Tile GROUP555_2[DB_BOARD_LEN];
extern const Tile GROUP555_3[DB_BOARD_LEN];

extern const Tile GROUP663_1[DB_BOARD_LEN];
extern const Tile GROUP663_2[DB_BOARD_LEN];
extern const Tile GROUP663_3[DB_BOARD_LEN];

/* Creates pattern database 5-5-5 */
void create_pattern_database555(void);

/* Creates pattern database 6-6-3 */
void create_pattern_database663(void);

/* Reads patter database into (global :( )map Board:int.
 * Destroy it after vie db_destroy */
void read_pattern_database(const char *filename);

/* Destroys database */
void db_destroy(void);

/* Returns cost for board if record exists. Otherwise returns -1 */
int db_get_cost(const Board board);

#endif
