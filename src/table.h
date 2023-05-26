#include "../data.h"
#include "../linked-list-ADT/list.h"

/* ================================================================ */

enum {
    IS_DESTROY_SET = (1 << 0),  /* 1 */
    IS_PRINT_SET = (1 << 1),    /* 2 */
    IS_MATCH_SET = (1 << 2),    /* 4 */
    IS_HASH_SET = (1 << 3),     /* 8 */
} _table_flags;

/* =============================== */
/* A pointer to an incomplete type */
/* =============================== */

typedef struct _hash_table* Table_t;

typedef ssize_t hash_v;

typedef hash_v (*fptr_hash)(const Data data, size_t table_capacity);

/* ================================================================ */

/* Allocate a new instance of a table */
extern Table_t Table_create(ssize_t buckets, fptr_hash hash, fptr_destroy destroy, fptr_print print, fptr_match match);

/* ================================================================ */

/* Deallocate a table. Delete its elements */
extern void Table_destroy(Table_t* table);

/* ================================================================ */

/* Search for an element in a table */
extern Data Table_find(const Table_t table, const Data data);

/* ================================================================ */

/* Insert data into a table */
extern int Table_insert(const Table_t table, const Data data);

/* ================================================================ */

/* Print the content of a table */    
extern void Table_print(const Table_t table);

/* ================================================================ */

/* Remove data form a table  */
extern int Table_remove(const Table_t table, const Data data);

/* ================================================================ */

/* Check if the specified flags are set */
extern uint8_t Table_is_flag_set(const Table_t table, uint8_t flags);