#include <stdlib.h>
#include <stdio.h>
#include "ale.h"

i32 main(void) {
    static u8 bufferzao[2*MBs_] = {0};
    Arena scratch = new_arena(isizeof(bufferzao), bufferzao);

    printf("%lld %lld", (i64) scratch.beg, (i64) scratch.end);

    return 0;
}
