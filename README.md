# Chained Hash Tables

## Content

[About](#about)

[API](#api)

[Examples](#examples)

[Sources](#sources)

## About

Hash tables support one of the most efficient types of searching: *hashing*. Fundamentally, a hash table consists of an array in which data is accessed via a special index called a *key*. The primary idea behind a hash table is to establish a mapping between the set of all possible keys and positions in the array using a *hash function*. A hash function accepts a key and returns its *hash coding*, or *hash value*.

Chained hash tables store data in *buckets*. Each bucket is a linked list that can grow as large as necessary to accommodate collisions.

## API

---

To work with tables, the following types are defined:

### hash_v

```C
typedef size_t hash_v;
```

The `hash_v` value represents a hash code of data that is needed to be inserted into a hash table.

### fptr_hash

```C
typedef hash_v (*fptr_hash)(const Data data, size_t table_capacity);
```

The `fptr_hash` is a callback function that computes a hash value of the given data. The `table_capacity` represents the number of positions into which elements may be hashed.

### _table_flags

A set of flags that can be retreived from a table:

| FLAG | Description |
| ---- | ----------- |
| IS_DESTROY_SET | Tells if there is a `destroy` function associated with the table |
| IS_PRINT_SET | Tells if there is a `print` function associated with the table |
| IS_MATCH_SET | Tells if there is a `match` function associated with the table |
| IS_HASH_SET | Tells if there is a `hash` function associated with the table |

See [examples](#examples) on how to write these callback functions.

---

Here go functions to work with a hash table

### Table_create

```C
Table_t Table_create(ssize_t buckets, fptr_hash hash, fptr_destroy destroy, fptr_print print, fptr_match match);
```

The `Table_create` function allocates a new instance of a table.

| Type | Description |
| ---- | ----------- |
| `ssize_t buckets` | Number of buckets into which elements may be hashed |
| `fptr_hash hash` | A callback function that computes a hash code when a user inserts, removes, or searches for the given data. Can't be `NULL` |
| `fptr_destroy destroy` | Provides a way to free dynamically allocated data when `Table_destroy` is called. If the table contains data dynamically allocated using `malloc`, `destroy` should be set to `free` to free the dataas th table is destroyed. For structured data containing several dynamically allocated members, destroy should be set to a user-defined function that calls `free` for each dynamically allocated member as well as for the structure itself. For a table containing data that should not be freed, destroy should be set to `NULL` |
| `fptr_print print` | Provides a way to display the table content |
| `fptr_match match` | Provides a way to compare data residing in a table |

**Return value**: a new instance of a table (`Table_t`) on success, `NULL` on failure.

### Table_destroy

```C
void Table_destroy(Table_t* table);
```

The `Table_destroy` function destroys a table.

| Type | Description |
| ---- | ----------- |
| `Table_t* table` | A pointer to a `Table_t` table to be deleted |

**Return value**: none. The table is `NULL` upon completion.

### Table_find

```C
Data Table_find(const Table_t table, const Data data);
```

The `Table_find` function searches for the specified data in the table.

| Type | Description |
| ---- | ----------- |
| `const Table_t table` | Table to search in |
| `const Data data` | Data to be searched |

**Return value**: specified data that resides in the table on success, `NULL` on failure.

### Table_insert

```C
int Table_insert(const Table_t table, const Data data);
```

The `Table_insert` function inserts data into a table.

| Type | Description |
| ---- | ----------- |
| `const Table_t table` | Table to insert into |
| `const Data data` | Data to be inserted |

**Return value**: *0* if the insertion is success, *-1* on failure.

### Table_print

```C
void Table_print(const Table_t table);
```

The `Table_print` function outputs the content of the table.

| Type | Description |
| ---- | ----------- |
| `const Table_t table` | Table to be printed |

**Return value**: none.

### Table_remove

```C
int Table_remove(const Table_t table, const Data data);
```

The `Table_remove` function deletes data from the table.

| Type | Description |
| ---- | ----------- |
| `const Table_t table` | Table to delete from |
| `const Data data` | Data to be deleted |

**Return value**: *0* if the deletion is success, *-1* on failure.

### Table_is_flag_set

```C
uint8_t Table_is_flag_set(const Table_t table, uint8_t flags);
```

The `Table_is_flag_set` function checks if specified flags `flags` are set on the given table `table`.

**Return value**: the same as `flags` if the set of specified flags are set on a given table.

## Examples

### How to write `hash` function

A hash function computes a hash key of the provided data. There are lots of algorithms to compute a key of different data types, so it's up to the user to pick one. Take a look at the following, artificial, example:

```C
hash_v hash_int(const Data data, size_t table_capacity) {
    hash_v code = -1;

    if (data != NULL) {
        code = ((*((int*) data) * 29) >> 2);
    }

    return code % table_capacity;
}
```

`code % table_capacity` gives us an index into which data must be stored.

### How to write `print` function

A print function, which is defined as:

```C
typedef void (*fptr_print)(const Data data);
```

provides a way to display data. Let's assume we have a struct representing a point in a 2D space, such as:

```C
typedef struct _point_2D* {
    int x;
    int y;
} Point2D_t;
```

To write a print function to handle such structures, you may do the following:

```C
void Point2D_print(const Data data) {
    if (point != NULL) {
        printf("[%d; %d]", ((Point2D_t) data)->x , ((Point2D_t) data)->y);
    }

    return ;
}
```

### How to write `match` function

Let's stick with the `Point2D_t` data structure. We define two pointers are equal, if they have the same coordinates:

```C
int Point2D_match(const Data data_1, const Data data_2) {
    /* Check if data_1 and data_2 are not NULL */

    return ( (((Point2D_t) data_1)->x == ((Point2D_t) data_2)->x) && (((Point2D_t) data_1)->y == ((Point2D_t) data_2)->y));
}
```

## Sources

[Mastering Algorithms with C](https://www.amazon.com/Mastering-Algorithms-Techniques-Sorting-Encryption/dp/1565924533/ref=sr_1_1?crid=1ZN4ENEQEGO5&keywords=mastering+c+algorithms&qid=1679619546&s=books&sprefix=mastering+c+algoritms%2Cstripbooks-intl-ship%2C221&sr=1-1) by Kyle Loudon.

[Data Structures, Algorithms, and Software Principles in C](https://www.amazon.com/Data-Structures-Algorithms-Software-Principles/dp/0201591189/ref=sr_1_10?crid=14MCGWUKW0F98&keywords=data+structures+and+algorithms+in+C&qid=1679784448&s=books&sprefix=data+structures+and+algorithms+in+c%2Cstripbooks-intl-ship%2C186&sr=1-10) by Thomas Standish.

[Computer Systems: A Programmer's Perspective](https://www.amazon.com/Computer-Systems-Programmers-Randal-Bryant/dp/013034074X/ref=sr_1_3?crid=1NDI8BHE4BPQM&keywords=computer+systems+a+programmer%27s+perspective&qid=1679784732&s=books&sprefix=computer+pers%2Cstripbooks-intl-ship%2C199&sr=1-3) by Randal E. Bryant, David Richard O'Hallaron.

[Patterns in C](https://leanpub.com/patternsinc) by Adam Tornhill
