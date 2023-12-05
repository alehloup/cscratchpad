#include <stdio.h>
#include <stdlib.h>
#include "ale.h"

typedef struct ds_header{Arena *arena; i32 elsize; i32 cap; i32 len;}ds_header;
_math_hot ds_header * hd_(void * ds) {
    return ((ds_header *) ds) - 1;
}
_fun_hot void * new_vec(Arena arena[1], i32 elsize) {
    ds_header * vec = alloc(arena, isizeof(ds_header) + (64*elsize), 1);
    vec->elsize = elsize;
    vec->cap = 64;
    vec->len = 0;
    vec->arena = arena;
    return vec + 1;
}
#define VNEW(arena, type) (type *) new_vec(arena, (i32)sizeof(type))

_fun_hot u8 * grow_vec(u8 * arr) { 
    ds_header * dh = hd_(arr);
    Arena *arena = dh->arena;
    cu8 *capend = &arr[dh->cap * dh->elsize];

    if ("VEC EXTEND" && arena->beg == capend) {
        u8 *VEC_EXTEND = (u8 *)alloc(arena, dh->elsize, dh->cap);
        
        assert((u64)VEC_EXTEND == (u64)capend && "extend misaligned");
    } else if ((u64)"VEC RELOC") {
        ds_header *VEC_RELOC = (ds_header *)
            alloc(arena, (dh->elsize*dh->cap * 2) + isizeof(ds_header), 1);
        u8 *newarr = (u8 *)(VEC_RELOC + 1);

        copymem(newarr, arr, dh->cap * dh->elsize);
        arr = newarr;
        copymem((u8 *)VEC_RELOC, (u8 *)dh, isizeof(ds_header));
        dh = VEC_RELOC;
    }
    dh->cap <<= 1;

    return arr;
}

_fun_hot void * push_vec(void * ptr_to_array) {
    u8 * *arr = (u8 * *)ptr_to_array;
    ds_header * dh = hd_(*arr);
    
    if (dh->len >= dh->cap) {
       *arr = grow_vec(*arr);
    }

    return *arr + (dh->elsize * dh->len++);
}
#define VAPPEND(arr) *(typeof(arr))push_vec(&arr)

int main(void) {
    static u8 buffer[1*MBs_] = {0};
    Arena arena = newarena(1*MBs_, buffer);

    i32 *x = VNEW(&arena, i32);
    printf("created array %d \n ", x[0]);

    for (i32 i = 0; i < 152; ++i) {
        printf("%d ", i);
        if (i > 64) {
            i32 *reloc = alloc(&arena, 4, 1);
            (*reloc) += 1;
        }
        VAPPEND(x) = i;
    } 

    for (int i = 0; i < hd_(x)->len; ++i) {
        printf("%d ", x[i]);
    }
    printf("\n");

    printf("%d %d %d", hd_(x)->elsize,  hd_(x)->cap, hd_(x)->len);
    
    return 0;
}
