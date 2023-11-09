#include <stdio.h>
#include <string.h>
#include "ale.h"

typedef struct teste{
    int x;
    long long y;
    int z;
}teste;

int main(int argc, char **argv) {{

    arena perm = newarena(_128MB, malloc(_128MB));

    printf("Começando beg:%p end:%p total:%lldMB\n", 
        perm.beg, perm.end, (perm.end - perm.beg)/1048576);

    teste *woa = newx(&perm, teste);
    *woa = (teste){4, 5, 6};
    printf("Ints: %d %lld %d\n", woa->x, woa->y, woa->z);

    teste *vetor = newxs(&perm, teste, 3);
    for(int i = 0, j = 0; i < 3; ++i) {
        vetor[i] = (teste){j++, j++, j++};
        printf("%d Ints: %d %lld %d\n", i, vetor[i].x, vetor[i].y, vetor[i].z);
    }

    return 0;
}}
