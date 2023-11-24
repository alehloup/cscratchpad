#include <stdlib.h>
#include <stdio.h>
#include "ale.h"

void test_ht_str(arena_t scratch) {
    ht64_t ht = new_ht64(&scratch, 4);
    
    ale_printf("%lld\n", hash_str("Alessandro Stamatto"));

    htstr_set_str(&ht, "Alessandro", "Stamatto");
    htstr_set_str(&ht, "Sarah", "Sakamotto");
    htstr_set_str(&ht, "Alessandro", "Ferreira");


    ale_printf("Pegou: %s\n", htstr_get_str(ht, "Sarah"));
    ale_printf("Não pegou: %s\n", htstr_get_str(ht, "Karol"));

    entry_str_str *data = htstr_data_as_str(&ht);
    for(int32_t i = 0; i < ht.mask; ++i) {
        if  (! data[i].key) {
            continue;
        }
        ale_printf("[%d]", i);
        ale_printf("%s: %s", data[i].key, data[i].val);
        ale_printf(", ");
    }
    ale_printf("\n");
}

void test_ht_i64(arena_t arena) {
    ht64_t ht = new_ht64(&arena, 4);

    ale_printf("%lld\n", hash_i64(4));

    htnum_set_str(&ht, 5, "Stamatto");
    htnum_set_str(&ht, 4, "Sakamoto");
    htnum_set_str(&ht, 5, "Ferreira");

    ale_printf("pegou: %s\n", htnum_get_str(ht, 5));
    ale_printf("não pegou: %s\n", htnum_get_str(ht, 3));

    entry_i64_str *data = htnum_data_as_str(&ht);
    for(int32_t i = 0; i < ht.mask; ++i) {
        if  ( ! data[i].key) {
            continue;
        }
        ale_printf("[%d]", i);
        ale_printf("%lld: %s", data[i].key, data[i].val);
        ale_printf(", ");
    }
    ale_printf("\n");
}

void test_big_ht_i64(arena_t scratch) {
    ht64_t ht = new_ht64(&scratch, 128);

    for(int i = 0; i < 128; ++i) {
        htnum_set_i64(&ht, i, i);
    }

    entry64_t *data = htnum_data_as_int64(&ht);
    for(int32_t i = 0; i < ht.len; ++i) {
        int32_t idx = htnum_get_idx(ht, i);
        
        ale_printf("[%d]", idx);
        ale_printf("%lld: %lld", data[idx].key, data[idx].val);
        ale_printf(", ");

        ale_assert(data[idx].key == data[idx].val, "should be equal!");
    }

    for(int32_t i = ht.len; i < ht.mask; ++i) {
        int32_t idx = htnum_get_idx(ht, i);

        ale_assert(data[idx].key == 0, "should be empty!");
    }


    ale_printf("\n");
}

int32_t main() {
    static uint8_t bufferzao[2*_Mega_Bytes] = {0};
    arena_t scratch = newarena(sizeof(bufferzao), bufferzao);
    test_ht_i64(scratch);
    ale_printf("\n");
    ale_printf("\n");
    test_ht_str(scratch);
    ale_printf("\n");
    ale_printf("\n");
    test_big_ht_i64(scratch);

    return 0;
}
