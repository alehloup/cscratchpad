#include <stdlib.h>
#include <stdio.h>
#include "ale.h"

_proc test_set(Arena arena) {
    i32 cap = 256 - 1;
    short * set = NEW_SET(&arena, short, cap);

    for (i32 i = 1; i < cap; ++i) {
        printf("%d ", i);
        assert(ht_set(set, &i) == i);
    }
    
    for (short i = 1; i < cap; ++i) {
        assert(ht_get(set, &i) == i);
    }
    
    printf("\nOkay!");
}

i32 main(void) {
    static u8 bufferzao[2*MBs_] = {0};
    Arena scratch = new_arena(isizeof(bufferzao), bufferzao);

    printf("\n");
    test_set(scratch);

    return 0;
}
