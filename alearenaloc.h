#pragma once

#include "ale.h"

#define ARENA_SIZE_ (1 << 28)

static __thread b32 GETARENA_uninit = 1;
arena* getarena() { //gets the thread Static Arena
    static __thread u8 mem[ARENA_SIZE_];
    static __thread arena r;
    if (GETARENA_uninit) {
        r = (arena){0};
        r.beg = mem;
        asm ("" : "+r"(r.beg)); //launders pointer
        r.end = r.beg + countof(mem);

        GETARENA_uninit = 0;
    }
    
    asm ("" : "+r"(r.beg)); //launders pointer
    return &r;
}
u8 *mallo(isize cap) { //malloc implemented using the static arena
    return (u8 *)alloc(getarena(), sizeof(u8), 16, cap);
}

arena newarena(isize cap) {
    arena a = {0};
    a.beg = (u8 *)mallo(cap); //can be replaced by malloc
    a.end = a.beg ? a.beg + cap : 0;
    return a;
}
