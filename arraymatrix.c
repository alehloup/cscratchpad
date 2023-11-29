#include <stdlib.h>
#include <stdio.h>
#include "ale.h"

static u8 * force_reloc = 0;

void test_vlamatrix(arena_t scratch) {
    printf("=========== MATRIXs ===========\n");

    i64 (*mat)[10][10] = (i64 (*)[10][10]) alloc(&scratch, sizeof(i64 [10][10]), 1);

    // test_size_cvla(10, 10, mat); // only works in C

    for(i32 i = 0; i < 10; ++i) {
        for(i32 j = 0; j < 10; ++j) {
            (*mat)[i][j] = i;
        }
    }

    for(i32 i = 0; i < 10; ++i) {
        for(i32 j = 0; j < 10; ++j) {
            printf("%lld ", (*mat)[i][j]);
        } 
        printf("\n");
    }
}

void test_vec_push_str(arena_t scratch) {
    printf("=========== CSTRINGs ===========\n");
    
    vector64_t d = {0, 0, 0};
    
    for (i32 i = 0; i < 52; ++i) {
        vec_push_str(&d, &scratch, "sAl");
        vec_push_str(&d, &scratch, "sSa");
        vec_push_str(&d, &scratch, "sEv");
    }

    cstr *data = vec_data_as_cstr(&d);
    for (i32 i = 0; i < d.len; ++i) {
        printf(" %d:%s ", i, data[i]);
    }

    printf("Pop: %s\n", vec_pop_str(&d));
}

void test_vec_push_i32(arena_t scratch) {
    printf("=========== I32s ===========\n");

    vector32_t d = {0, 0, 0};
    
    vec_push_i32(&d, &scratch, 52+32000);
    printf("POP: %d\n", vec_pop_i32(&d));

    for(i32 i = 0; i < 164; ++i) {
        vec_push_i32(&d, &scratch, i+32000);
    }

    printf("POP: %d\n", vec_pop_i32(&d));

    i32 *data = vec_data_as_i32(&d);
    for(i32 i = 0; i < d.len; ++i) {
        printf("%d ", data[i]);
    } 
    printf("\n");
}

void test_vec_push_i64(arena_t scratch) {
    printf("=========== I64s ===========\n");

    vector64_t d = {0, 0, 0};
    
    vec_push_i64(&d, &scratch, 52+64000);
    printf("POP: %lld\n", vec_pop_i64(&d));

    for(i32 i = 0; i < 164; ++i) {
        force_reloc = (u8 *) alloc(&scratch, sizeof(u8), 1);
        *force_reloc = 52;
        vec_push_i64(&d, &scratch, i+64000);
    }

    printf("POP: %lld\n", vec_pop_i64(&d));

    i64 *data = vec_data_as_i64(&d);

    for(i32 i = 0; i < d.len; ++i) {
        printf("%lld ", data[i]);
    } 
    printf("\n");
}

void test_vec_push_f32(arena_t scratch) {
    printf("=========== FLOATs ===========\n");

    vector32_t d = {0, 0, 0};
    
    vec_push_f32(&d, &scratch, 5.2f + 0.0032f);
    printf("POP: %f\n", (f64) vec_pop_f32(&d));

    for(i32 i = 0; i < 164; ++i) {
        vec_push_f32(&d, &scratch, ((f32)i / 2.0f) + 0.0032f);
    }

    printf("POP: %f\n", (f64) vec_pop_f32(&d));

    f32 *data = vec_data_as_f32(&d);
    for(i32 i = 0; i < d.len; ++i) {
        printf("%f ", (f64) data[i]);
    } 
    printf("\n");
}

void test_vec_push_f64(arena_t scratch) {
    printf("=========== DOUBLEs ===========\n");
    
    vector64_t d = {0, 0, 0};
    
    vec_push_f64(&d, &scratch, 5.2+0.0064);
    printf("POP: %lf\n", vec_pop_f64(&d));

    for(i32 i = 0; i < 164; ++i) {
        vec_push_f64(&d, &scratch, (i / 2.0)+0.0064);
        force_reloc = (u8 *) alloc(&scratch, sizeof(u8), 1);
        *force_reloc = 3;
    }

    printf("POP: %lf\n", vec_pop_f64(&d));

    f64 *data = vec_data_as_f64(&d);
    for(i32 i = 0; i < d.len; ++i) {
        printf("%lf ", data[i]);
    } 
    printf("\n");
}

i32 main() {

    arena_t scratch = newarena(8*_Mega_Bytes, (u8 *) calloc(8*_Mega_Bytes, 1));
    
    test_vlamatrix(scratch);
    test_vec_push_str(scratch);
    test_vec_push_i32(scratch);
    test_vec_push_i64(scratch);
    test_vec_push_f32(scratch);
    test_vec_push_f64(scratch);

    return 0;
}
