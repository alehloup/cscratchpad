#include <stdlib.h>
#include <stdio.h>
#include "ale.h"

typedef struct teste{
    i32 x;
    long long y;
    i32 z;
}teste;

i32 main(void) {

    Arena perm = new_arena(1*MBs_, (u8 *)calloc(1*MBs_, 1));

    teste *vetor = (teste *) alloc(&perm, isizeof(teste), 3);
    teste *woa = (teste *) alloc(&perm, isizeof(teste), 1);
    teste temp = {4, 5, 6};

    printf("Começando beg:%p end:%p total:%lldMB\n", 
        (void *) perm.beg, (void *) perm.end, (perm.end - perm.beg)/1048576);

    *woa =temp;
    printf("Ints: %d %lld %d\n", woa->x, woa->y, woa->z);

    for(i32 i = 0, j = 0; i < 3; ++i) {
        teste newel = {j, j+1, j+2};
        j += 3;
        vetor[i] = newel;
        printf("%d Ints: %d %lld %d\n", i, vetor[i].x, vetor[i].y, vetor[i].z);
    }

    return 0;
}
