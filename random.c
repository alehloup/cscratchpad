#include <stdio.h>
#include "ale.h"

i32 main() {
    u64 seed = 41635984;

    #define N 20
    i32 counts[N] = {0};
    for(i32 i = 0; i < 1000000000; ++i) {
        counts[rnd(&seed) % N] +=1;
    }
    for(i32 i = 0; i < N; ++i) {
        printf("%d: %d\n", i+1, counts[i]);
    }

    return 0;
}
