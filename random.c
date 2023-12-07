#include "ale_io.h"

i32 main(void) {
    u64 seed = 41635984;

    #define N 20
    i32 counts[N] = {0};

    clock_t start = clock();
    for(i32 i = 0; i < 1000000000; ++i) {
        counts[rnd(&seed) % N] +=1;
    }
    {
        for(i32 i = 0; i < N; ++i) {
            printf("%d: %d\n", i+1, counts[i]);
        }
        print_clock(start);
    }

    return 0;
}
