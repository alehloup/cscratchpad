#include <stdlib.h>
#include <stdio.h>
#include "ale.h"

_proc test_set(Arena arena) {
    i32 *ivec = NEW_VEC(&arena, i32);
    printf("is_string: %d cap:%d\n", hd_(ivec)->is_str, hd_(ivec)->cap);

    i32 *ivec2 = NEW_VEC_PW2(&arena, i32, 256);
    printf("is_string: %d cap:%d\n", hd_(ivec2)->is_str, hd_(ivec2)->cap);

    cstr *svec = NEW_VEC(&arena, cstr);
    printf("is_string: %d cap:%d\n", hd_(svec)->is_str, hd_(svec)->cap);

    mstr *svec2 = NEW_VEC_PW2(&arena, mstr, 700);
    printf("is_string: %d cap:%d\n", hd_(svec2)->is_str, hd_(svec2)->cap);

    char * *svec3 = NEW_VEC_PW2(&arena, char *, 269);
    printf("is_string: %d cap:%d\n", hd_(svec3)->is_str, hd_(svec3)->cap);
}

i32 main(void) {
    static u8 bufferzao[2*MBs_] = {0};
    Arena scratch = new_arena(isizeof(bufferzao), bufferzao);

    test_set(scratch);

    return 0;
}
