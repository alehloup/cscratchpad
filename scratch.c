#include <stdio.h>
#include <stdlib.h>
#include "ale.h"

typedef struct ds_header{i32 elsize; i32 cap; i32 len;}ds_header;
_fun void * new_vec(Arena arena[1], i32 elsize) {
    ds_header * vec = alloc(arena, (i64)sizeof(ds_header) + (64*elsize), 1);
    vec->elsize = elsize;
    vec->cap = 64;
    vec->len = 0;
    return vec + 1;
}

_pure ds_header * hd_(void * ds) {
    return ((ds_header *) ds) - 1;
}



int main(void) {
    u8 buffer[256] = {0};
    Arena arena = newarena(256, buffer);
    i8 *x = (i8 *) new_vec(&arena, sizeof(i8));
    x[0] = 5;
    x[1] = 2;
    x[2] = 6;
    x[3] = 9;

    for (int i = 0; i < 4; ++i) {
        printf("%d ", x[i]);
    }
    printf("\n");

    printf("%d %d %d", hd_(x)->elsize,  hd_(x)->cap, hd_(x)->len);
    
    return 0;
}
