#include "./aleh/system.h"

int main(void) {
    unsigned int i;
    size_t seed = 2147483269;

    #define N 20
    int counts[N] = {0};
    (void) counts;

    start_benchclock();

    srand((unsigned int)seed);

    for(i = 0; i < 1000; ++i) {
        unsigned int x = ((unsigned int) rand()) % N;
        counts[x] +=1;
    }
    
    for(i = 0; i < N; ++i) {
        printf("%u: %d\n", i+1, counts[i]);
    }
    
    stop_benchclock();

    printf("\nTested random functions.\n");

    return 0;
}
