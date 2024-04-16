#include <time.h>
#include <stdio.h>
#include "ale.h"

int32_t main(void) {
    unsigned int64_t seed = 41635984;

    #define N 20
    int32_t counts[N] = {0};

    clock_t start = clock();
    for(int32_t i = 0; i < 1000000000; ++i) {
        counts[rnd(&seed) % N] +=1;
    }
    {
        for(int32_t i = 0; i < N; ++i) {
            printf("%d: %d\n", i+1, counts[i]);
        }
        print_clock(start);
    }

    return 0;
}
