# Lib Dynamic Array

Header-struct based Heap allocated dynamic length array.

```c
// darr.h - implementations

// all functions and macros are NULL safe.
// with the exception of darr_free() all functions set errno=EINVAL if NULL
// is passed.

// initialize a new dynamic array using using type and initial count
// create a new int array w/ 10 slots allocated, and a count of 0 
//     int *my_int_darr = DARR_INIT(int, 10);
//
#define DARR_INIT(t, n) (t *)(darr_init (_Alignof (t), (n)))


// core initializer function, identical to DARR_INIT except that darr_init()
// takes a sizeof(type) instead of type directly
// refer to DARR_INIT() for example.
void *darr_init(size_t type_size, size_t n);


// increment count by n, resize array if count surpasses allocated size, return
// new array pointer. If new count is less than allocate size, return s_arr.
// on error, returns NULL, but do NOT free s_arr.
//     int *arr = DARR_INIT(int, 0);
//     void *tmp = darr_push(arr, 2);
//     if (tmp == NULL) {
//         // handle error
//         return;
//     }
//     arr = tmp;
//     arr[0] = 6;
//     arr[1] = 7;
//
void *darr_push(void *s_arr, size_t n);


// decrement count by 1, and return a pointer to the popped value; pointer is
// valid until next darr function call.
// on error, return NULL
void *darr_pop(void *s_arr);


// decrement count by n and return s_arr. On error return NULL.
void *darr_slide(void *s_arr, size_t n);


// get the number of elements used in a dynamic array
size_t darr_count(void *s_arr);


// clean up a dynamic array allocated using either DARR_INIT() or darr_init()
void darr_free(void *s_arr);
```

## Example Program

```c
#include <darr.h>

#include <assert.h>
#include <readline/readline.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define DARR(arr) ((void *)arr)

int main(void) {
    char **str_arr = DARR_INIT(char *, 1); // initialize array of strings
    size_t i = 0;
    char *input_str;

    assert(str_arr != NULL);

    do {
        // read a line of data
        input_str = readline("> ");
        if (input_str == NULL) { // do not add NULLs to the array
            continue;
        }

        // increment dynamic array count, safely
        void *tmp = darr_push(DARR(str_arr, 1);
        if (tmp == NULL) {
            break; // on error, stop adding items, go straight to printing
        }
        str_arr = (char **)tmp;
        str_arr[i++] = input_str;
    } while (input_str == NULL || *input_str != '\0'); // loop until an empty line is input

    // remove trailing empty line
    tmp = darr_pop(DARR(str_arr));
    if (tmp != NULL) {
        free(*(char **)tmp);
    }

    // print lines last in first out
    for (i = darr_count(DARR(str_arr)); i > 0; i--) {
        char **p_read_str = (char **)darr_pop(DARR(str_arr));
        if (p_read_str == NULL) {
            continue;
        }
        printf("%s\n", *read_str);
        free(*read_str); // after printing the line, free it's memory
    }

    darr_free(DARR(str_arr)); // cleanup remaining heap objects
    return 0;
}

/*
> testline 1
> line 2
> line 3
> 
line 3
line 2
testline 1
*/
```
