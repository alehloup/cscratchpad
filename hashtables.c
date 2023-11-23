#include <stdlib.h>
#include <stdio.h>
#include "ale.h"

void test_msi_cstr(arena_t scratch) {
    msiht64 ht = newmsi64(&scratch, 4);
    
    ale_printf("%lld\n", hash_cstr("Alessandro Stamatto"));

    msiks_set_cstr(&ht, "Alessandro", "Stamatto");
    msiks_set_cstr(&ht, "Sarah", "Sakamotto");
    msiks_set_cstr(&ht, "Alessandro", "Ferreira");


    ale_printf("Pegou: %s\n", msiks_get_cstr(ht, "Sarah"));
    ale_printf("Não pegou: %s\n", msiks_get_cstr(ht, "Karol"));

    entry_cstr_cstr *data = msiks_data_as_cstr(&ht);
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

void test_msi_i64(arena_t arena) {
    msiht64 ht = newmsi64(&arena, 4);

    ale_printf("%lld\n", hash_i64(4));

    msiki_set_cstr(&ht, 5, "Stamatto");
    msiki_set_cstr(&ht, 4, "Sakamoto");
    msiki_set_cstr(&ht, 5, "Ferreira");

    ale_printf("pegou: %s\n", msiki_get_cstr(ht, 5));
    ale_printf("não pegou: %s\n", msiki_get_cstr(ht, 3));

    entry_i64_cstr *data = msiki_data_as_cstr(&ht);
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

void test_big_msi_i64(arena_t scratch) {
    msiht64 ht = newmsi64(&scratch, 128);

    for(int i = 0; i < 128; ++i) {
        msiki_set_i64(&ht, i, i);
    }

    entry_i64_i64 *data = msiki_data_as_int64(&ht);
    for(int32_t i = 0; i < ht.len; ++i) {
        int32_t idx = msiki_get_idx(ht, i);
        
        ale_printf("[%d]", idx);
        ale_printf("%lld: %lld", data[idx].key, data[idx].val);
        ale_printf(", ");

        ale_assert(data[idx].key == data[idx].val, "should be equal!");
    }

    for(int32_t i = ht.len; i < ht.mask; ++i) {
        int32_t idx = msiki_get_idx(ht, i);

        ale_assert(data[idx].key == 0, "should be empty!");
    }


    ale_printf("\n");
}

static uint8_t bufferzao[2*_Mega_Bytes] = {};
int32_t main() {
    arena_t scratch = newarena(sizeof(bufferzao), bufferzao);
    test_msi_i64(scratch);
    ale_printf("\n");
    ale_printf("\n");
    test_msi_cstr(scratch);
    ale_printf("\n");
    ale_printf("\n");
    test_big_msi_i64(scratch);

    return 0;
}
