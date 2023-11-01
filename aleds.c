//#pragma once
#include <stdio.h>
#include "alearena.h"



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

    u64 s = *((u64 *)malloc(sizeof(u64)));
    i32 counts[6] = {0, 0, 0, 0, 0, 0};

    fori(10000000) {
        counts[RND64(s) % 6] +=1;
    }
    fori(6) {
        printf("%lld: %ld\n", i, counts[i]);
    }

    return 0;
}
