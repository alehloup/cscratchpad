#include <stdlib.h>
#include <stdio.h>
#include "ale.h"

static u8 * force_reloc = 0;

_proc test_vec_push_str(Arena scratch) {
    cstr *cstrings = VNEW(&scratch, cstr);

    printf("=========== CSTRINGs ===========\n");
    
    for (i32 i = 0; i < 52; ++i) {
        VAPPEND(cstrings) = "sAl";
        VAPPEND(cstrings) = "sSa";
        VAPPEND(cstrings) = "sEv";
    }

    {
        for (i32 i = 0; i < hd_(cstrings)->len; ++i) {
            printf(" %d:%s ", i, cstrings[i]);
        }
    }

    printf("Pop: %s\n", VPOP(cstrings));
}

_proc test_vec_push_i64(Arena scratch) {
    i64 *i64s = VNEW(&scratch, i64);

    printf("=========== I64s ===========\n");
    
    VAPPEND(i64s) = 52+64000;
    printf("POP: %lld\n", VPOP(i64s));

    for(i32 i = 0; i < 164; ++i) {
        force_reloc = (u8 *) alloc(&scratch, isizeof(u8), 1);
        *force_reloc = 52;
        VAPPEND(i64s) = i+64000;
    }

    printf("POP: %lld\n", VPOP(i64s));

    for(i32 i = 0; i < hd_(i64s)->len; ++i) {
        printf("%lld ", i64s[i]);
    } 
    printf("\n");
}

_proc test_vec_push_f64(Arena scratch) {
    f64 *f64s = VNEW(&scratch, f64);

    printf("=========== DOUBLEs ===========\n");
    
    VAPPEND(f64s) = 5.2+0.0064;
    printf("POP: %lf\n", VPOP(f64s));

    for(i32 i = 0; i < 164; ++i) {
        VAPPEND(f64s) = (i / 2.0)+0.0064;
        force_reloc = (u8 *) alloc(&scratch, isizeof(u8), 1);
        *force_reloc = 3;
    }

    printf("POP: %lf\n", VPOP(f64s));
    
    for(i32 i = 0; i < hd_(f64s)->len; ++i) {
        printf("%lf ", f64s[i]);
    } 
    printf("\n");
}

i32 main(void) {

    Arena scratch = newarena(8*MBs_, (u8 *) calloc(8*MBs_, 1));
    
    test_vec_push_str(scratch);
    test_vec_push_i64(scratch);
    test_vec_push_f64(scratch);

    return 0;
}
