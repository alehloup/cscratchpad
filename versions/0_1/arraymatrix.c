#include <stdlib.h>
#include <stdio.h>
#include "ale.h"

static u8 * force_reloc = 0;

_proc test_vec_push_str(Arena scratch) {
    NEW_VEC_WITH_CAP(&scratch, cstrings, cstr, 5);

    printf("=========== CSTRINGs ===========\n");
    
    for (i32 i = 0; i < 52; ++i) {
        vec_append(cstrings, "sAl");
        vec_append(cstrings, "sSa");
        vec_append(cstrings, "sEv");
    }

    {
        for (i32 i = 0; i < hd_(cstrings)->len; ++i) {
            printf(" %d:%s ", i, cstrings[i]);
        }
    }

    printf("Pop: %s\n", vec_pop(cstrings));
}

_proc test_vec_push_i64(Arena scratch) {
    NEW_VEC_WITH_CAP(&scratch, i64s, i64, 3);

    printf("=========== I64s ===========\n");
    
    vec_append(i64s, 52+64000);
    printf("POP: %lld\n", vec_pop(i64s));

    printf("checkptr before: %d %d %llu\n", hd_(i64s)->ptrcheck, (u8)(u64)(i64s), (u64)(i64s));
    assert(hd_checkptr(i64s));
    for(i32 i = 0; i < 164; ++i) {
        force_reloc = (u8 *) alloc(&scratch, isizeof(u8), 2);
        *force_reloc = 52;
        vec_append(i64s, i+64000);
    }
    assert(hd_checkptr(i64s));
    printf("checkptr after: %d %d %llu\n", hd_(i64s)->ptrcheck, (u8)(u64)(i64s), (u64)(i64s));

    printf("POP: %lld\n", vec_pop(i64s));

    for(i32 i = 0; i < hd_(i64s)->len; ++i) {
        printf("%lld ", i64s[i]);
    } 
    printf("\n");
}

_proc test_vec_push_f64(Arena scratch) {
    NEW_VEC(&scratch, f64s, f64);

    printf("=========== DOUBLEs ===========\n");
    
    vec_append(f64s, 5.2+0.0064);
    printf("POP: %lf\n", vec_pop(f64s));

    for(i32 i = 0; i < 164; ++i) {
        vec_append(f64s, (i / 2.0)+0.0064);
        force_reloc = (u8 *) alloc(&scratch, isizeof(u8), 1);
        *force_reloc = 3;
    }

    printf("POP: %lf\n", vec_pop(f64s));
    
    for(i32 i = 0; i < hd_(f64s)->len; ++i) {
        printf("%lf ", f64s[i]);
    } 
    printf("\n");
}

i32 main(void) {

    Arena scratch = new_arena(8*MBs_, (u8 *) calloc(8*MBs_, 1));
    
    test_vec_push_str(scratch);
    test_vec_push_i64(scratch);
    test_vec_push_f64(scratch);

    return 0;
}
