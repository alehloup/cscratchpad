#include <stdlib.h>
#include <stdio.h>
#include "ale.h"

typedef struct teste{
    int32_t x;
    long long y;
    int32_t z;
}teste;

int32_t main() {{

    arena_t perm = newarena(128*_Mega_Bytes, (uint8_t *) calloc(128*_Mega_Bytes, 1));

    printf("Começando beg:%p end:%p total:%lldMB\n", 
        perm.beg, perm.end, (perm.end - perm.beg)/1048576);

    teste *woa = (typeof(woa)) alloc(&perm, sizeof(teste), 1);
    teste temp = {4, 5, 6};
    *woa =temp;
    printf("Ints: %d %lld %d\n", woa->x, woa->y, woa->z);

    teste *vetor = (typeof(vetor)) alloc(&perm, sizeof(teste), 3);
    for(int32_t i = 0, j = 0; i < 3; ++i) {
        teste newel = {j++, j++, j++};
        vetor[i] = newel;
        printf("%d Ints: %d %lld %d\n", i, vetor[i].x, vetor[i].y, vetor[i].z);
    }

    return 0;
}}
