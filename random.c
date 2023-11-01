#include <stdio.h>
#include <stdlib.h>
#include "ale.h"

int main() {
    fori(5) {
        forj(3) {
            printf("|%lld %lld|", i, j);
        }
      printf("\n");  
    } 

    forrange(x, 10, 0, -1) {
        printf("%lld ", x);
    } printf("\n");

    RNDSEED(malloc(64));

    #define N 20
    i32 counts[N] = {0};
    fori(100'000'000) {
        counts[RND64() % N] +=1;
    }
    fori(N) {
        printf("%lld: %ld\n", i+1, counts[i]);
    }

    return 0;
}
