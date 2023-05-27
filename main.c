#include "src/table.h"
#include <time.h>

ssize_t hash_int(const Data data, size_t table_capacity);

void print_int(const Data data);

int int_match(const Data data_1, const Data data_2) {
    return (*((int*) data_1) - *((int*) data_2));
}

int main(int argc, char** argv) {
    Table_t table = NULL;

    time_t t;

    srand((unsigned) time(&t));

    int array[] = {1, 3, 5, 99, 54, 23, 590, 35, 23};

    if ((table = Table_create(11, hash_int, NULL, print_int, int_match)) != NULL) {

        for (size_t i = 0; i < sizeof(array) / sizeof(array[0]); i++) {
            Table_insert(table, &array[i]);
        }

        Table_print(table);

        Table_remove(table, (array + 3));
        Table_print(table);

        if (Table_is_flag_set(table, IS_PRINT_SET | IS_HASH_SET | IS_MATCH_SET | IS_DESTROY_SET)) {
            printf("Ok\n");
        }
    }

    Table_destroy(&table);

    Table_print(table);

    return EXIT_SUCCESS;
}

ssize_t hash_int(const Data data, size_t table_capacity) {
    return ((*((int*) data) ^ 321) % table_capacity);
}

void print_int(const Data data) {
    printf("%d", *((int*) data));

    return ;
}