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

    return 0;
}
