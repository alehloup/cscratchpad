#include <stdlib.h>
#include <stdio.h>
#include "ale.h"

_proc test_hashtable_skey(Arena arena) {
    i32 cap = 256 - 1;
    NEW_HTABLE(&arena, table, cstr, int, cap);

    for (i32 i = 1; i < cap; ++i) {
        mstr skey = ALLOCN(&arena, char, 16);
        sprintf_s(skey, 16, "\"%d\"", i);
        printf("%s ", skey);
        
        assert(htable_set(table, skey, i) == i);
    }
    assert(hd_len_(table_keys) == cap - 1);

    for (i32 i = 1; i < cap; ++i) {
        mstr skey = ALLOCN(&arena, char, 16);
        sprintf_s(skey, 16, "\"%d\"", i);
        printf("%s: %d ", skey, htable_get(table, skey));
        
        assert(htable_get(table, skey) == i);
    }

    for (i32 i = 1; i < cap; ++i) {
        mstr skey = ALLOCN(&arena, char, 16);
        sprintf_s(skey, 16, "\"%d\"", i);
        printf("%s ", skey);
        
        assert(htable_set(table, skey, i) == i);
    }
    assert(hd_len_(table_keys) == cap - 1);

    for (i32 i = 1; i < cap; ++i) {
        mstr skey = ALLOCN(&arena, char, 16);
        sprintf_s(skey, 16, "\"%d\"", i);
        printf("%s: %d ", skey, htable_get(table, skey));
        
        assert(htable_get(table, skey) == i);
    }

    printf("\nOkay!\n");
}

_proc test_set_strs(Arena arena) {
    cstr *set = NEW_SET(&arena, cstr, 0);

    discard_ hset_set(set, "Alex");
    discard_ hset_set(set, "Sarah");
    discard_ hset_set(set, "Karol");
    discard_ hset_set(set, "Amanda");
    discard_ hset_set(set, "Ingridy");

    printf("%s %s %s %s\n", 
        hset_get(set, "Alex"), hset_get(set, "Sarah"), hset_get(set, "amanda"), hset_get(set, "karol"));
    printf("Okay!\n");
}

_proc test_set_skey(Arena arena) {
    i32 cap = 256 - 1;
    cstr *set = NEW_SET(&arena, cstr, cap);

    for (i32 i = 1; i < cap; ++i) {
        mstr skey = ALLOCN(&arena, char, 16);
        sprintf_s(skey, 16, "\"%d\"", i);
        printf("%s ", skey);
        assert(!cstrcmp(hset_set(set, skey), skey));
    }
    assert(hd_len_(set) == cap - 1);

    for (i32 i = 1; i < cap; ++i) {
        mstr skey = ALLOCN(&arena, char, 16);
        sprintf_s(skey, 16, "\"%d\"", i);
        assert(!cstrcmp(hset_get(set, skey), skey));
    }

    for (i32 i = 1; i < cap; ++i) {
        mstr skey = ALLOCN(&arena, char, 16);
        sprintf_s(skey, 16, "\"%d\"", i);
        printf("%s ", skey);
        assert(!cstrcmp(hset_set(set, skey), skey));
    }
    assert(hd_len_(set) == cap - 1);

    for (i32 i = 1; i < cap; ++i) {
        mstr skey = ALLOCN(&arena, char, 16);
        sprintf_s(skey, 16, "\"%d\"", i);
        assert(!cstrcmp(hset_get(set, skey), skey));
    }

    for (i32 i = cap; i < cap * 2; ++i) {
        mstr skey = ALLOCN(&arena, char, 16);
        sprintf_s(skey, 16, "\"%d\"", i);
        assert(hset_get(set, skey) == 0);
    }

    printf("\nOkay!\n");
}

_proc test_set_ikey(Arena arena) {
    i32 cap = 256 - 1;
    short * set = NEW_SET(&arena, short, cap);

    for (i32 i = 1; i < cap; ++i) {
        printf("%d ", i);
        assert(hset_set(set, i) == i);
    }
    assert(hd_len_(set) == cap - 1);

    for (short i = 1; i < cap; ++i) {
        assert(hset_get(set, i) == i);
    }

    printf("\n\n");
    for (i32 i = 1; i < cap; ++i) {
        printf("%d ", i);
        assert(hset_set(set, i) == i);
    }
    assert(hd_len_(set) == cap - 1);
    
    for (short i = 1; i < cap; ++i) {
        assert(hset_get(set, i) == i);
    }
    
    for (int i = cap; i < cap * 2; ++i) {
        assert(hset_get(set, i) == 0);
    }

    printf("\nOkay!\n");
}

i32 main(void) {
    static u8 bufferzao[2*MBs_] = {0};
    Arena scratch = new_arena(isizeof(bufferzao), bufferzao);

    printf("\n");
    test_set_ikey(scratch);
    test_set_skey(scratch);
    test_set_strs(scratch);
    test_hashtable_skey(scratch);

    return 0;
}
