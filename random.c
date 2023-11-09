#include "ale.h"

int main() {

    SET_RND_SEED((uint64) malloc(64));

    #define N 20
    int counts[N] = {0};
    for(int i = 0; i < 1000000000; ++i) {
        counts[RND64() % N] +=1;
    }
    for(int i = 0; i < N; ++i) {
        printf("%d: %d\n", i+1, counts[i]);
    }

    return 0;
}
