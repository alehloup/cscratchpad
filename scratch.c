#include <stdio.h>
#include <stdlib.h>
#include "ale.h"

int main(void) {
    static u8 buffer[1*MBs_] = {0};
    Arena arena = new_arena(1*MBs_, buffer);

    i32 *keys = NEWPW2VEC(&arena, i32, 512);
    cstr *vals = NEWPW2VEC(&arena, cstr, 512);

    printf("%d %d %d \n", hd_(keys)->cap, fit_pwr2_exp(hd_(keys)->cap + 1), hd_(vals)->cap);

    return 0;
}
