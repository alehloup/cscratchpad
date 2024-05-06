#include "ale.h"

int main(void) {
    size_t seed = 41635984;

    #define N 20
    int counts[N] = {0};
    (void) counts;

    start_benchclock();

    for(unsigned int i = 0; i < 1000000000; ++i) {
        counts[rnd(&seed) % N] +=1;
    }
    
    for(unsigned int i = 0; i < N; ++i) {
        printf("%d: %d\n", i+1, counts[i]);
    }
    
    stop_benchclock();

    printf("\nTested random functions.\n");

    return 0;
}
