#include <stdlib.h>
#include <stdio.h>
#include "ale.h"

void test_ht_str(arena_t scratch) {
    ht64_t ht = new_ht64(&scratch, 4);
    
    printf("%lld\n", hash_str("Alessandro Stamatto"));

    htstr_set_str(&ht, "Alessandro", "Stamatto");
    htstr_set_str(&ht, "Sarah", "Sakamotto");
    htstr_set_str(&ht, "Alessandro", "Ferreira");


    printf("Pegou: %s\n", htstr_get_str(ht, "Sarah"));
    printf("Não pegou: %s\n", htstr_get_str(ht, "Karol"));

    entry_str_str *data = htstr_data_as_str(&ht);
    for(i32 i = 0; i < ht.mask; ++i) {
        if  (! data[i].key) {
            continue;
        }
        printf("[%d]", i);
        printf("%s: %s", data[i].key, data[i].val);
        printf(", ");
    }
    printf("\n");
}

void test_ht_i64(arena_t arena) {
    ht64_t ht = new_ht64(&arena, 4);

    printf("%lld\n", hash_i64(4));

    htnum_set_str(&ht, 5, "Stamatto");
    htnum_set_str(&ht, 4, "Sakamoto");
    htnum_set_str(&ht, 5, "Ferreira");

    printf("pegou: %s\n", htnum_get_str(ht, 5));
    printf("não pegou: %s\n", htnum_get_str(ht, 3));

    entry_i64_str *data = htnum_data_as_str(&ht);
    for(i32 i = 0; i < ht.mask; ++i) {
        if  ( ! data[i].key) {
            continue;
        }
        printf("[%d]", i);
        printf("%lld: %s", data[i].key, data[i].val);
        printf(", ");
    }
    printf("\n");
}

void test_big_ht_i64(arena_t scratch) {
    ht64_t ht = new_ht64(&scratch, 128);

    for(i32 i = 0; i < 128; ++i) {
        htnum_set_i64(&ht, i, i);
    }

    entry64_t *data = htnum_data_as_int64(&ht);
    for(i32 i = 0; i < ht.len; ++i) {
        i32 idx = htnum_get_idx(ht, i);
        
        printf("[%d]", idx);
        printf("%lld: %lld", data[idx].key, data[idx].val);
        printf(", ");

        _assert_(data[idx].key == data[idx].val && "should be equal!");
    }

    for(i32 i = ht.len; i < ht.mask; ++i) {
        i32 idx = htnum_get_idx(ht, i);

        _assert_(data[idx].key == 0 && "should be empty!");
    }


    printf("\n");
}

i32 main() {
    static u8 bufferzao[2*_MBs] = {0};
    arena_t scratch = newarena(sizeof(bufferzao), bufferzao);
    test_ht_i64(scratch);
    printf("\n");
    printf("\n");
    test_ht_str(scratch);
    printf("\n");
    printf("\n");
    test_big_ht_i64(scratch);

    return 0;
}
