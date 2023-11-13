#include <stdlib.h>
#include <stdio.h>
#include "ale.h"

typedef struct teste{
    int32_t x;
    long long y;
    int32_t z;
}teste;

int32_t main(int32_t argc, cstring *argv) {{

    arena perm = newarena(128*MegaBytes, malloc(128*MegaBytes));

    printf("Começando beg:%p end:%p total:%lldMB\n", 
        perm.beg, perm.end, (perm.end - perm.beg)/1048576);

    teste *woa = newx(&perm, teste);
    *woa = cpound(teste){4, 5, 6};
    printf("Ints: %d %lld %d\n", woa->x, woa->y, woa->z);

    teste *vetor = newxs(&perm, teste, 3);
    for(int32_t i = 0, j = 0; i < 3; ++i) {
        vetor[i] = cpound(teste){j++, j++, j++};
        printf("%d Ints: %d %lld %d\n", i, vetor[i].x, vetor[i].y, vetor[i].z);
    }

    return 0;
}}
