#include <stdlib.h>
#include <stdio.h>
#include "ale.h"

void test_msi_cstr(arena a) {
    auto ht = new_64msi(&a, 4);
    auto data = ht->data;
    
    ale_printf("%lld\n", hash_cstr("Alessandro Stamatto"));

    data[msi_set_skey(ht, "Alessandro")].val = (cstrtoi) "Stamatto";
    data[msi_set_skey(ht, "Sarah")].val = (cstrtoi) "Sakamotto";
    data[msi_set_skey(ht, "Alessandro")].val = (cstrtoi) "Ferreira";

    ale_printf("Pegou: %s\n", (itocstr) data[msi_get_by_skey(ht, "Sarah")].val);
    ale_printf("Não pegou: %s\n", (itocstr) data[msi_get_by_skey(ht, "Karol")].val);

    for(int32_t i = 0; i < ht->mask; ++i) {
        if  (! data[i].key) {
            continue;
        }
        ale_printf("[%d]", i);
        ale_printf("%s: %s", (itocstr) data[i].key, (itocstr) data[i].val);
        ale_printf(", ");
    }
    ale_printf("\n");
}

void test_msi_i64(arena a) {
    auto ht = new_64msi(&a, 4);
    auto data = ht->data;

    ale_printf("%lld\n", hash_i64(4));

    data[msi_set_i64key(ht, 5)].val = (cstrtoi) "Stamatto";
    data[msi_set_i64key(ht, 4)].val = (cstrtoi) "Sakamoto";
    data[msi_set_i64key(ht, 5)].val = (cstrtoi) "Ferreira";

    ale_printf("pegou: %s\n", (itocstr) data[msi_get_by_i64key(ht, 5)].val);
    ale_printf("não pegou: %s\n", (itocstr) data[msi_get_by_i64key(ht, 3)].val);

    for(int32_t i = 0; i < ht->mask; ++i) {
        if  ( ! data[i].key) {
            continue;
        }
        ale_printf("[%d]", i);
        ale_printf("%lld: %s", data[i].key, (itocstr) data[i].val);
        ale_printf(", ");
    }
    ale_printf("\n");
}

void test_big_msi_i64(arena a) {
    auto ht = new_64msi(&a, 128);
    auto data = ht->data;

    for(int i = 0; i < 128; ++i) {
        data[msi_set_i64key(ht, i)].val = i;
    }

    for(int32_t i = 0; i < ht->len; ++i) {
        int32_t idx = msi_get_by_i64key(ht, i);
        
        ale_printf("[%d]", idx);
        ale_printf("%lld: %lld", data[idx].key, data[idx].val);
        ale_printf(", ");

        ale_assert(data[idx].key == data[idx].val, "should be equal!");
    }

    for(int32_t i = ht->len; i < ht->mask; ++i) {
        int32_t idx = msi_get_by_i64key(ht, i);

        ale_assert(data[idx].key == 0, "should be empty!");
    }


    ale_printf("\n");
}

static thread_local uint8_t bufferzao[2*_Mega_Bytes];
int32_t main() {
    arena scratch = newarena(sizeof(bufferzao), bufferzao);
    test_msi_i64(scratch);
    ale_printf("\n");
    ale_printf("\n");
    test_msi_cstr(scratch);
    ale_printf("\n");
    ale_printf("\n");
    test_big_msi_i64(scratch);

    return 0;
}
