#include <stdlib.h>
#include "ale.h"

typedef struct i64s{
    int32_t cap; int32_t len; int64_t *data;
}i64s;
void test_push_i64(arena scratch) {
    i64s d = {0};
    
    push_i64(&d, &scratch, 52);
    printf("POP: %lld\n", pop_i64(&d));

    for(int32_t i = 0; i < 164; ++i) {
        push_i64(&d, &scratch, i);
    }

    printf("POP: %lld\n", pop_i64(&d));

    for(int32_t i = 0; i < d.len; ++i) {
        printf("%lld ", d.data[i]);
    } 
    printf("\n");
}

typedef struct cstrings{
    int32_t cap; int32_t len; char * *data;
} cstrings;
void test_push_cstr(arena scratch) {
    cstrings d = {0};
    
    push_cstr(&d, &scratch, "Alessandro");
    push_cstr(&d, &scratch, "Sarah");
    push_cstr(&d, &scratch, "Evelyn");

    for (int i = 0; i < d.len; ++i) {
        printf(" Str: %s\n", d.data[i]);
    }

    printf("Pop: %s\n", pop_cstr(&d));
}

typedef struct s8s{
    int32_t cap; int32_t len; s8 *data;
} s8s;
void test_push_s8(arena scratch) {
    s8s d = {0};
    
    push_s8(&d, &scratch, s("s8 Alessandro"));
    push_s8(&d, &scratch, s("s8 Sarah"));
    push_s8(&d, &scratch, s("s8 Evelyn"));

    for (int i = 0; i < d.len; ++i) {
        printf(" Str: %s\n", d.data[i].data);
    }

    printf("Pop: %s\n", pop_s8(&d).data);
}

typedef struct doubles{
    int32_t cap; int32_t len; double *data;
}doubles;
void test_push_double(arena scratch) {
    doubles d = {0};
    
    push_double(&d, &scratch, 5.2);
    printf("POP: %lf\n", pop_double(&d));

    for(int32_t i = 0; i < 164; ++i) {
        push_double(&d, &scratch, i / 2.0);
    }

    printf("POP: %lf\n", pop_double(&d));

    for(int32_t i = 0; i < d.len; ++i) {
        printf("%lf ", d.data[i]);
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
    
    test_vlamatrix(scratch);

    test_push_i64(scratch);
    test_push_cstr(scratch);
    test_push_s8(scratch);
    test_push_double(scratch);

    return 0;
}
