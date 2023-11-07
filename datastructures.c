#include "ale.h"

typedef struct at{
    caplendata(i64);
}at;
void test_push(arena scratch) {
    at d = {};

    fori(164) {
        *push(&d, &scratch) = i;
    }

    fori(d.len) {
        printf("%lld ", d.data[i]);
    } 
    printn;
}

void test_vlamatrix(arena a) {
    NEWMATRIX_VAR(i64, mat, 10, 10, &a);

    fori(10) {
        forj(10) {
            (*mat)[i][j] = i;
        }
    }

    fori(10) {
        forj(10) {
            printf("%lld ", matat(mat, i, j));
        } 
        printf("\n");
    }
}

int main() {

    arena scratch = newarena(_8MB);
    test_push(scratch);
    test_vlamatrix(scratch);

    return 0;
}
