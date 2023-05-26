#include "table.h"

/* ================================================================ */

/* ================================= */
/* An implementation of a hash table */
/* ================================= */

struct _hash_table {
    /* Number of buckets in a table */
    size_t n_buckets;

    /* Number of elements/entries in the table */
    size_t size;

    /* Flags containig information about table 'methods' */
    uint8_t flags;

    /* An array of buckets */
    List_t* buckets;

    /* A pointer to a hash function */
    fptr_hash hash;
};

/* ================================================================ */
/* ========================== Interface =========================== */
/* ================================================================ */

Table_t Table_create(ssize_t buckets, fptr_hash hash, fptr_destroy destroy, fptr_print print, fptr_match match) {
    Table_t table = NULL;

    /* ====================================================== */
    /* Every hash table must have an associated hash function */
    /* ====================================================== */

    if (hash == NULL) {
        warn_with_user_msg(__func__, "a hash function must be provided");

        return table;
    }
    
    /* ============================================= */
    /* A number of buckets can't be a negative value */
    /* ============================================= */

    if (buckets < 0) {
        warn_with_user_msg(__func__, "a number of buckets can't be negative");

        return table;
    }

    /* ================================================================ */

    /* Allocate memory for a table container */
    if ((table = (Table_t) malloc(sizeof(struct _hash_table))) != NULL) {

        table->n_buckets = buckets;
        table->size = 0;
        table->flags = 0;
        table->hash = hash;

        /* Set table flags */
        table->flags |= (destroy != NULL) ? IS_DESTROY_SET : 0;
        table->flags |= (print != NULL) ? IS_PRINT_SET : 0;
        table->flags |= (match != NULL) ? IS_MATCH_SET : 0;
        table->flags |= IS_HASH_SET;

        /* ======================================== */
        /* Allocate an array of pointers to buckets */
        /* ======================================== */

        if ((table->buckets = (List_t*) malloc(sizeof(struct _linked_list) * table->n_buckets)) != NULL) {
            
            /* ============================= */
            /* Allocate an individual bucket */
            /* ============================= */

            for (size_t i = 0; i < table->n_buckets; i++) {
                if ((*(table->buckets + i) = List_create(destroy, print, match)) == NULL) {
                    /* If memory allocation failure occurs */
                    warn_with_sys_msg(__func__);

                    /* Delete the table */
                    Table_destroy(&table);
                }
            }

            return table;
        }
        else {
            warn_with_sys_msg(__func__);
        }
    }
    else {
        warn_with_sys_msg(__func__);
    }

    /* Delete the table if something went wrong */
    Table_destroy(&table);

    return table;
}

/* ================================================================ */

void Table_destroy(Table_t* table) {
    /* Do not dereference NULL */
    if ((table != NULL) && (*table != NULL)) {

        for (size_t i = 0; i < (*table)->n_buckets; i++) {
            /* Yeap! That's a very peculiar syntax :D */
            List_destroy(&(*((*table)->buckets + i)));
        }

        /* =========== */
        /* Cleaning up */
        /* =========== */

        /* Free an array of buckets first  */
        free((*table)->buckets);

        /* Clear the memory */
        memset(*table, 0, sizeof(struct _hash_table));

        /* free memory occipied by a table */
        free(*table);

        /* Explicitly setthe a table to NULL */
        *table = NULL;
    }

    return ;
}

/* ================================================================ */

Data Table_find(const Table_t table, const Data data) {
    /* Data to be returned */
    Data found_data = NULL;

    /* A hash value of data */
    hash_v code;

    Node node = NULL;

    /* ================ */

    /* Do not dereference NULL */
    if (table != NULL) {

        /* Compute a hash code of data */
        code = table->hash(data, table->n_buckets);

        /* Search the data in the bucket at index code */
        node = List_find(*(table->buckets + code), data);
        
        /* Check if node is not NULL */
        found_data = (node != NULL) ? node->data : NULL;
    }
    else {
        warn_with_user_msg(__func__, "provided table is NULL");
    }

    /* ================ */

    return found_data;
}

/* ================================================================ */

int Table_insert(const Table_t table, const Data data) {
    int result = -1;

    /* A hash value of data */
    hash_v code = 0;

    /* ================ */

    /* Do not dereference NULL */
    if (table != NULL) {
        /* Make sure a user doesn't provide NULL as data */
        if (data != NULL) {
            
            /* Compute a hash code of data */
            code = table->hash(data, table->n_buckets);

            if ((result = List_insert_last(*(table->buckets + code), data)) == 0) {
                table->size++;
            }
        }
        else {
            warn_with_user_msg(__func__, "data can't be NULL");
        }
    }
    else {
        warn_with_user_msg(__func__, "provided table is NULL");
    }

    /* ================ */

    return result;
}

/* ================================================================ */

extern void Table_print(const Table_t table) {
    if (table != NULL) {

        /* ============================================================= */
        /* There is not associated `print` function with the given table */
        /* ============================================================= */

        if ((table->flags & IS_PRINT_SET) == 0) {
            warn_with_user_msg(__func__, "there is no accosiated print function with the given table");

            return ;
        }

        /* ================ */

        for (size_t i = 0; i < table->n_buckets; i++) {
            printf("(%zu) ", i);

            List_print(*(table->buckets + i), NULL);
        }
    }

    /* ================ */

    return ;
}

/* ================================================================ */

int Table_remove(const Table_t table, const Data data) {
    int result = -1;

    /* Data to be remove */
    Data removed_data = NULL;

    /* A hash value of data */
    hash_v code;

    /* ================ */

    /* Do not dereference NULL */
    if (table != NULL) {
        /* Make sure a user doesn't provide NULL as data */
        if (data != NULL) {
            
            /* Compute a hash code of data */
            code = table->hash(data, table->n_buckets);
            
            /* Find a node with the specified data */
            if ((removed_data = List_remove_node(*(table->buckets + code), List_find(*(table->buckets + code), data))) != NULL) {

                /* Delete data if needed */
                if ((table->flags & IS_DESTROY_SET) > 0) {
                    free(removed_data);
                }

                result = 0;
            }
        }
    }

    /* ================ */
    
    return result;
}

/* ================================================================ */

uint8_t Table_is_flag_set(const Table_t table, uint8_t flags) {
    return (table != NULL) ? ((table->flags & flags) == flags) : 0;
}