#include "ale.h"

int main() {
    unsigned long long seed = 41635984;

    #define N 20
    int counts[N] = {0};
    for(int i = 0; i < 1000000000; ++i) {
        counts[RND(&seed) % N] +=1;
    }
    for(int i = 0; i < N; ++i) {
        printf("%d: %d\n", i+1, counts[i]);
    }

    return 0;
}
