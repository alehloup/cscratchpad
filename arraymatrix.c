#include <stdlib.h>
#include <stdio.h>
#include "ale.h"

void test_vlamatrix(arena a) {
    ale_printf("=========== MATRIXs ===========\n");

    int64_t (*mat)[10][10] = (typeof(mat)) alloc(&a, sizeof(int64_t [10][10]), alignof(int64_t [10][10]), 1);

    // test_size_cvla(10, 10, mat); // only works in C

    for(int32_t i = 0; i < 10; ++i) {
        for(int32_t j = 0; j < 10; ++j) {
            (*mat)[i][j] = i;
        }
    }

    for(int32_t i = 0; i < 10; ++i) {
        for(int32_t j = 0; j < 10; ++j) {
            ale_printf("%lld ", (*mat)[i][j]);
        } 
        ale_printf("\n");
    }
}

void test_push_cstr(arena scratch) {
    ale_printf("=========== CSTRINGs ===========\n");
    
    vector64 d = {};
    
    for (int i = 0; i < 52; ++i) {
        push_cstr(&d, &scratch, "sAl");
        auto force_reloc = (int8_t *) alloc(&scratch, sizeof(int8_t), alignof(int8_t), 1);
        push_cstr(&d, &scratch, "sSa");
        push_cstr(&d, &scratch, "sEv");
    }

    auto data = vector_data_as_cstring(&d);
    for (int i = 0; i < d.len; ++i) {
        ale_printf(" %d:%s ", i, data[i]);
    }

    ale_printf("Pop: %s\n", pop_cstr(&d));
}

void test_push_i32(arena scratch) {
    ale_printf("=========== I32s ===========\n");

    vector32 d = {};
    
    push_i32(&d, &scratch, 52+32000);
    ale_printf("POP: %d\n", pop_i32(&d));

    for(int32_t i = 0; i < 164; ++i) {
        push_i32(&d, &scratch, i+32000);
        auto force_reloc = (int8_t *) alloc(&scratch, sizeof(int8_t), alignof(int8_t), 1);
    }

    ale_printf("POP: %d\n", pop_i32(&d));

    auto data = vector_data_as_i32(&d);
    for(int32_t i = 0; i < d.len; ++i) {
        ale_printf("%d ", data[i]);
    } 
    ale_printf("\n");
}

void test_push_i64(arena scratch) {
    ale_printf("=========== I64s ===========\n");

    vector64 d = {};
    
    push_i64(&d, &scratch, 52+64000);
    ale_printf("POP: %lld\n", pop_i64(&d));

    for(int32_t i = 0; i < 164; ++i) {
        push_i64(&d, &scratch, i+64000);
        auto force_reloc = (int8_t *) alloc(&scratch, sizeof(int8_t), alignof(int8_t), 1);
    }

    ale_printf("POP: %lld\n", pop_i64(&d));

    auto data = vector_data_as_i64(&d);

    for(int32_t i = 0; i < d.len; ++i) {
        ale_printf("%lld ", data[i]);
    } 
    ale_printf("\n");
}

void test_push_f32(arena scratch) {
    ale_printf("=========== FLOATs ===========\n");

    vector32 d = {};
    
    push_f32(&d, &scratch, 5.2f + 0.0032f);
    ale_printf("POP: %f\n", (double) pop_f32(&d));

    for(int32_t i = 0; i < 164; ++i) {
        push_f32(&d, &scratch, ((float)i / 2.0f) + 0.0032f);
        auto force_reloc = (int8_t *) alloc(&scratch, sizeof(int8_t), alignof(int8_t), 1);
    }

    ale_printf("POP: %f\n", (double) pop_f32(&d));

    auto data = vector_data_as_f32(&d);
    for(int32_t i = 0; i < d.len; ++i) {
        ale_printf("%f ", (double) data[i]);
    } 
    ale_printf("\n");
}

void test_push_f64(arena scratch) {
    ale_printf("=========== DOUBLEs ===========\n");
    
    vector64 d = {};
    
    push_f64(&d, &scratch, 5.2+0.0064);
    ale_printf("POP: %lf\n", pop_f64(&d));

    for(int32_t i = 0; i < 164; ++i) {
        push_f64(&d, &scratch, (i / 2.0)+0.0064);
        auto force_reloc = (int8_t *) alloc(&scratch, sizeof(int8_t), alignof(int8_t), 1);
    }

    ale_printf("POP: %lf\n", pop_f64(&d));

    auto data = vector_data_as_f64(&d);
    for(int32_t i = 0; i < d.len; ++i) {
        ale_printf("%lf ", data[i]);
    } 
    ale_printf("\n");
}

int32_t main() {

    arena scratch = newarena(8*_Mega_Bytes, (uint8_t *) malloc(8*_Mega_Bytes));
    
    test_vlamatrix(scratch);
    test_push_cstr(scratch);
    test_push_i32(scratch);
    test_push_i64(scratch);
    test_push_f32(scratch);
    test_push_f64(scratch);

    return 0;
}
