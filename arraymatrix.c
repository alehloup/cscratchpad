#include <stdlib.h>
#include <stdio.h>
#include "ale.h"

void test_vlamatrix(arena_t scratch) {
    printf("=========== MATRIXs ===========\n");

    int64_t (*mat)[10][10] = (int64_t (*)[10][10]) alloc(&scratch, sizeof(int64_t [10][10]), 1);

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

void test_vec_push_str(arena_t scratch) {
    printf("=========== CSTRINGs ===========\n");
    
    vector64_t d = {0, 0, 0};
    
    for (int i = 0; i < 52; ++i) {
        vec_push_str(&d, &scratch, "sAl");
        vec_push_str(&d, &scratch, "sSa");
        vec_push_str(&d, &scratch, "sEv");
    }

    int64_t *data = vec_data_as_i64(&d);
    for (int i = 0; i < d.len; ++i) {
        printf(" %d:%s ", i, (cstr_t) data[i]);
    }

    printf("Pop: %s\n", vec_pop_str(&d));
}

void test_vec_push_i32(arena_t scratch) {
    printf("=========== I32s ===========\n");

    vector32_t d = {0, 0, 0};
    
    vec_push_i32(&d, &scratch, 52+32000);
    printf("POP: %d\n", vec_pop_i32(&d));

    for(int32_t i = 0; i < 164; ++i) {
        vec_push_i32(&d, &scratch, i+32000);
        int8_t * force_reloc = (int8_t *) alloc(&scratch, sizeof(int8_t), 1);
        *force_reloc = 52;
    }

    printf("POP: %d\n", vec_pop_i32(&d));

    int32_t *data = vec_data_as_i32(&d);
    for(int32_t i = 0; i < d.len; ++i) {
        printf("%d ", data[i]);
    } 
    printf("\n");
}

void test_vec_push_i64(arena_t scratch) {
    printf("=========== I64s ===========\n");

    vector64_t d = {0, 0, 0};
    
    vec_push_i64(&d, &scratch, 52+64000);
    printf("POP: %lld\n", vec_pop_i64(&d));

    for(int32_t i = 0; i < 164; ++i) {
        vec_push_i64(&d, &scratch, i+64000);
    }

    printf("POP: %lld\n", vec_pop_i64(&d));

    int64_t *data = vec_data_as_i64(&d);

    for(int32_t i = 0; i < d.len; ++i) {
        printf("%lld ", data[i]);
    } 
    printf("\n");
}

void test_vec_push_f32(arena_t scratch) {
    printf("=========== FLOATs ===========\n");

    vector32_t d = {0, 0, 0};
    
    vec_push_f32(&d, &scratch, 5.2f + 0.0032f);
    printf("POP: %f\n", (float64_t) vec_pop_f32(&d));

    for(int32_t i = 0; i < 164; ++i) {
        vec_push_f32(&d, &scratch, ((float32_t)i / 2.0f) + 0.0032f);
    }

    printf("POP: %f\n", (float64_t) vec_pop_f32(&d));

    float32_t *data = vec_data_as_f32(&d);
    for(int32_t i = 0; i < d.len; ++i) {
        printf("%f ", (float64_t) data[i]);
    } 
    printf("\n");
}

void test_vec_push_f64(arena_t scratch) {
    printf("=========== DOUBLEs ===========\n");
    
    vector64_t d = {0, 0, 0};
    
    vec_push_f64(&d, &scratch, 5.2+0.0064);
    printf("POP: %lf\n", vec_pop_f64(&d));

    for(int32_t i = 0; i < 164; ++i) {
        vec_push_f64(&d, &scratch, (i / 2.0)+0.0064);
    }

    printf("POP: %lf\n", vec_pop_f64(&d));

    float64_t *data = vec_data_as_f64(&d);
    for(int32_t i = 0; i < d.len; ++i) {
        printf("%lf ", data[i]);
    } 
    printf("\n");
}

int32_t main() {

    arena_t scratch = newarena(8*_Mega_Bytes, (uint8_t *) malloc(8*_Mega_Bytes));
    
    test_vlamatrix(scratch);
    test_vec_push_str(scratch);
    test_vec_push_i32(scratch);
    test_vec_push_i64(scratch);
    test_vec_push_f32(scratch);
    test_vec_push_f64(scratch);

    return 0;
}
