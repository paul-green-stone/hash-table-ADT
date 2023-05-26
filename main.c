#include "src/table.h"
#include <time.h>

ssize_t hash_int(const Data data, size_t table_capacity);

void print_int(const Data data);

int int_match(const Data data_1, const Data data_2) {
    return (*((int*) data_1) - *((int*) data_2));
}

int main(int argc, char** argv) {
    Table_t table = NULL;

    int* x = NULL;

    time_t t;

    srand((unsigned) time(&t));

    int array[] = {1, 3, 5, 99, 54, 23, 590, 35, 23};

    if ((table = Table_create(11, hash_int, NULL, print_int, int_match)) != NULL) {

        for (size_t i = 0; i < 9; i++) {
            Table_insert(table, &array[i]);
        }

        Table_print(table);
    }

    Table_remove(table, &array[5]);

    Table_print(table);

    Table_destroy(&table);

    return EXIT_SUCCESS;
}

ssize_t hash_int(const Data data, size_t table_capacity) {
    return ((*((int*) data) ^ 321) % table_capacity);
}

void print_int(const Data data) {
    printf("%d", *((int*) data));

    return ;
}