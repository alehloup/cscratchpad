#pragma once

#include "aledefs.h"

tstruct(arena) {
    u8 *beg;
    u8 *end;
};

__attribute((malloc, alloc_size(2, 4), alloc_align(3)))
void *alloc(arena *a, isize size, isize align, isize count) {
    isize total = size * count;
    isize pad = MODPWR2(- (isize)a->beg, align); //mod -x gives n for next align
    
    if (count > (a->end - a->beg - pad)/size) {
        assert(count <= (a->end - a->beg - pad)/size); //ARENA OUT OF MEMORY
        return 0; //ARENA OUT OF MEMORY
    }

    u8 *p = a->beg + pad;
    a->beg += pad + total;
    
    return ZEROMEM(p, total);
}

#define newx(a, t) (t *)alloc(a, sizeof(t), alignof(t), 1)
#define newxs(a, t, n) (t *)alloc(a, sizeof(t), alignof(t), n)

#define ARENA_SIZE_ (1 << 28)
static b32 GETARENA_uninit = 1;
arena* getarena() { //gets the only Static Arena
    static u8 mem[ARENA_SIZE_];
    static arena r;
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
u8 *mallo(isize cap) { //malloc implemented using the static arena!
    return (u8 *)alloc(getarena(), sizeof(u8), 16, cap);
}

arena newarena(isize cap) {
    arena a = {0};
    a.beg = (u8 *)mallo(cap);
    a.end = a.beg ? a.beg + cap : 0;
    return a;
}
