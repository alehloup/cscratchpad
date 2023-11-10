#include <stdlib.h>
#include "ale.h"

typedef struct at{
    int32_t cap; int32_t len; int64_t *data;
}at;
void test_push(arena scratch) {
    at d = {0};
    
    push(&d, &scratch, 52);
    printf("POP: %lld\n", pop(&d));

    for(int32_t i = 0; i < 164; ++i) {
        push(&d, &scratch, i);
    }

    printf("POP: %lld\n", pop(&d));

    for(int32_t i = 0; i < d.len; ++i) {
        printf("%lld ", d.data[i]);
    } 
    printf("\n");
}

void test_size(int m, int n, int64_t mat[static 1][m][n]) {
    printf("size: %lld \n", sizeof(*mat));
}

void test_vlamatrix(arena a) {
    int64_t (*mat)[10][10] = newmat(&a, int64_t, 10, 10);

    test_size(10, 10, mat);

    for(int32_t i = 0; i < 10; ++i) {
        for(int32_t j = 0; j < 10; ++j) {
            (*mat)[i][j] = i;
        }
    }

    for(int32_t i = 0; i < 10; ++i) {
        for(int32_t j = 0; j < 10; ++j) {
            printf("%lld ", (*mat)[i][j]);
        } 
        printf("\n");
    }
}

int32_t main() {

    arena scratch = newarena(8*MegaBytes, malloc(8*MegaBytes));
    test_push(scratch);
    test_vlamatrix(scratch);

    return 0;
}
