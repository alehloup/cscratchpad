#pragma once

#include <string.h> //for memset
#include <stdlib.h> //for malloc
#include "aledefs.h"

tstruct(arena) {
    u8 *beg;
    u8 *end;
};

__attribute((malloc, alloc_size(2, 4), alloc_align(3)))
void *alloc(arena *a, isize size, isize align, isize count) {
    isize total = size * count;
    isize pad = MODPWR2(- (isize)a->beg); //mod -x gives n for next align
    
    if (count > (a->end - a->beg - pad)/size) {
        assert(count <= (a->end - a->beg - pad)/size); //ARENA OUT OF MEMORY
        return 0; //ARENA OUT OF MEMORY
    }

    u8 *p = a->beg + pad;
    a->beg += pad + total;
    
    return memset(p, 0, total);
}

#define newx(a, t) (t *)alloc(a, sizeof(t), alignof(t), 1)
#define newxs(a, t, n) (t *)alloc(a, sizeof(t), alignof(t), n)

arena newarena(isize cap) {
    arena a = {0};
    a.beg = (u8 *)malloc(cap);
    a.end = a.beg ? a.beg + cap : 0;
    return a;
}

#define ARENA_SIZE_ (1 << 2) //not being used, could not launder the pointer =p
arena getarena() {
    static u8 mem[ARENA_SIZE_];
    arena r = {0};
    r.beg = mem;
    r.end = r.beg + countof(mem);

    //asm ("" : "+r"(r.beg));  // launder the pointer
    //__builtin_launder(r.beg);
    return r;
}
