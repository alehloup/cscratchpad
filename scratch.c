#include <stdio.h>
#include <stdlib.h>
#include "ale.h"

int main(void) {
    static u8 buffer[1*MBs_] = {0};
    Arena arena = new_arena(1*MBs_, buffer);

    NEWHTABLE(alex, &arena, i32, 64);

    printf("%d %d %d \n", hd_(alex_keys)->cap, fit_pwr2_exp(hd_(alex_vals)->cap + 1), hd_(alex_vals)->cap);

    return 0;
}
