#include "ale.h"

typedef struct at{
    caplendata(long long);
}at;
void test_push(arena scratch) {
    at d = {0};
    
    push(&d, &scratch, 52);
    print("POP: %lld", pop(&d));

    for(int i = 0; i < 164; ++i) {
        push(&d, &scratch, i);
    }

    print("POP: %lld", pop(&d));

    for(int i = 0; i < d.len; ++i) {
        printf("%lld ", d.data[i]);
    } 
    printn;
}

void test_vlamatrix(arena a) {
    NEWMATRIX_VAR(long long, mat, 10, 10, &a);

    for(int i = 0; i < 10; ++i) {
        for(int j = 0; j < 10; ++j) {
            (*mat)[i][j] = i;
        }
    }

    for(int i = 0; i < 10; ++i) {
        for(int j = 0; j < 10; ++j) {
            printf("%lld ", matat(mat, i, j));
        } 
        printf("\n");
    }
}

int main() {

    arena scratch = newarena(_8MB, malloc(_8MB));
    test_push(scratch);
    test_vlamatrix(scratch);

    return 0;
}
