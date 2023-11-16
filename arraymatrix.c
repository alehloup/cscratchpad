#include <stdlib.h>
#include <stdio.h>
#include "ale.h"

typedef struct i64s{
    int32_t cap; int32_t len; int64_t *data;
}i64s;
void test_push_i64(arena scratch) {
    printf("=========== I64s ===========\n");

    i64s d = {};
    
    push_i64(&d, &scratch, 52+64000);
    printf("POP: %lld\n", pop_i64(&d));

    for(int32_t i = 0; i < 164; ++i) {
        push_i64(&d, &scratch, i+64000);
        int8_t *force_reloc = (int8_t *) alloc(&scratch, sizeof(int8_t), alignof(int8_t), 1);
    }

    printf("POP: %lld\n", pop_i64(&d));

    for(int32_t i = 0; i < d.len; ++i) {
        printf("%lld ", d.data[i]);
    } 
    printf("\n");
}

typedef struct cstrings{
    int32_t cap; int32_t len; cstring *data;
} cstrings;
void test_push_cstr(arena scratch) {
    printf("=========== CSTRINGs ===========\n");
    
    cstrings d = {};
    
    for (int i = 0; i < 52; ++i) {
        push_cstr(&d, &scratch, "sAl");
        int8_t *force_reloc = (int8_t *) alloc(&scratch, sizeof(int8_t), alignof(int8_t), 1);
        push_cstr(&d, &scratch, "sSa");
        push_cstr(&d, &scratch, "sEv");
    }

    for (int i = 0; i < d.len; ++i) {
        printf(" %d:%s ", i, d.data[i]);
    }

    printf("Pop: %s\n", pop_cstr(&d));
}

typedef struct doubles{
    int32_t cap; int32_t len; double *data;
}doubles;
void test_push_double(arena scratch) {
    printf("=========== DOUBLEs ===========\n");
    
    doubles d = {};
    
    push_double(&d, &scratch, 5.2+0.00064);
    printf("POP: %lf\n", pop_double(&d));

    for(int32_t i = 0; i < 164; ++i) {
        push_double(&d, &scratch, (i / 2.0)+0.00064);
        int8_t *force_reloc = (int8_t *) alloc(&scratch, sizeof(int8_t), alignof(int8_t), 1);
    }

    printf("POP: %lf\n", pop_double(&d));

    for(int32_t i = 0; i < d.len; ++i) {
        printf("%lf ", d.data[i]);
    } 
    printf("\n");
}

// void test_size_cvla(int m, int n, int64_t mat[_at_least_(1)][m][n]) {
//     printf("size: %lld \n", sizeof(*mat));
// }


void test_vlamatrix(arena a) {
    printf("=========== MATRIXs ===========\n");

    int64_t (*mat)[10][10] = (typeof(mat)) alloc(&a, sizeof(int64_t [10][10]), alignof(int64_t [10][10]), 1);

    // test_size_cvla(10, 10, mat); // only works in C

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

typedef struct i32s{
    int32_t cap; int32_t len; int32_t *data;
}i32s;
void test_push_i32(arena scratch) {
    printf("=========== I32s ===========\n");

    i32s d = {};
    
    push_i32(&d, &scratch, 52+32000);
    printf("POP: %d\n", pop_i32(&d));

    for(int32_t i = 0; i < 164; ++i) {
        push_i32(&d, &scratch, i+32000);
        int8_t *force_reloc = (int8_t *) alloc(&scratch, sizeof(int8_t), alignof(int8_t), 1);
    }

    printf("POP: %d\n", pop_i32(&d));

    for(int32_t i = 0; i < d.len; ++i) {
        printf("%d ", d.data[i]);
    } 
    printf("\n");
}

typedef struct floats{
    int32_t cap; int32_t len; float *data;
}floats;
void test_push_float(arena scratch) {
    printf("=========== FLOATs ===========\n");

    floats d = {};
    
    push_float(&d, &scratch, 5.2f + 0.0032f);
    printf("POP: %f\n", pop_float(&d));

    for(int32_t i = 0; i < 164; ++i) {
        push_float(&d, &scratch, ((float)i / 2.0f) + 0.0032f);
        int8_t *force_reloc = (int8_t *) alloc(&scratch, sizeof(int8_t), alignof(int8_t), 1);
    }

    printf("POP: %f\n", pop_float(&d));

    for(int32_t i = 0; i < d.len; ++i) {
        printf("%f ", (double) d.data[i]);
    } 
    printf("\n");
}

int32_t main() {

    arena scratch = newarena(8*MegaBytes, malloc(8*MegaBytes));
    
    test_vlamatrix(scratch);
    test_push_cstr(scratch);
    test_push_i32(scratch);
    test_push_i64(scratch);
    test_push_float(scratch);
    test_push_double(scratch);

    return 0;
}
