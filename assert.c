#include <stdlib.h>
#include <stdio.h>
#include "ale.h"

typedef struct teste{
    i32 x;
    long long y;
    i32 z;
}teste;

i32 main() {{

    arena_t perm = newarena(1, (u8 *)calloc(1, 1));

    printf("Começando beg:%p end:%p total:%lldMB\n", 
        (void *) perm.beg, (void *) perm.end, (perm.end - perm.beg)/1048576);

    teste *woa = (teste *) alloc(&perm, sizeof(teste), 1);
    teste temp = {4, 5, 6};
    *woa =temp;
    printf("Ints: %d %lld %d\n", woa->x, woa->y, woa->z);

    teste *vetor = (teste *) alloc(&perm, sizeof(teste), 3);
    for(i32 i = 0, j = 0; i < 3; ++i) {
        teste newel = {0};
        newel.x = j++;
        newel.y = j++;
        newel.z = j++;
        vetor[i] = newel;
        printf("%d Ints: %d %lld %d\n", i, vetor[i].x, vetor[i].y, vetor[i].z);
    }

    return 0;
}}
