#include <stdio.h>
#include <string.h>
#include "alearena.h"

tstruct(teste) {
    i32 x;
    i64 y;
    i8 z;
};

int main(int argc, char **argv) {
    arena perm = newarena(1 << 28);

    printf("Começando beg:%p end:%p total:%lldMB\n", 
        perm.beg, perm.end, (perm.end - perm.beg)/1048576);

    teste *woa = newx(&perm, teste);
    *woa = (teste){4, 5, 6};
    printf("Ints: %ld %lld %d\n", woa->x, woa->y, woa->z);

    teste *vetor = newxs(&perm, teste, 3);
    for(i8 i = 0, j = 0; i < 3; ++i) {
        vetor[i] = (teste){j++, j++, j++};
        printf("%d Ints: %ld %lld %d\n", i, vetor[i].x, vetor[i].y, vetor[i].z);
    }

    return 0;
}
