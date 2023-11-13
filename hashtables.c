#include <stdlib.h>
#include <stdio.h>
#include "ale.h"

typedef struct strht{ 
    int32_t stepshift;int32_t capmask; 
    int32_t len; struct{cstring key; cstring val;} *data; 
}strht;

void test_msi_cstr(arena a) {
    strht *ht = (strht *)newmsi(&a, 5);
    typeof(ht->data) data = ht->data;

    printf("%lld\n", hash_cstr("Alessandro Stamatto"));

    data[msi_set_by_skey(ht, "Alessandro")].val = "Stamatto";
    data[msi_set_by_skey(ht, "Sarah")].val = "Sakamotto";
    data[msi_set_by_skey(ht, "Alessandro")].val = "Ferreira";

    printf("Pegou: %s\n", data[msi_get_by_skey(ht, "Sarah")].val);
    printf("Não pegou: %s\n", data[msi_get_by_skey(ht, "Karol")].val);

    for(int32_t i = 0; i < ht->capmask; ++i) {
        if  (! data[i].key) {
            continue;
        }
        printf("[%d]", i);
        printf("%s: %s", data[i].key, data[i].val);
        printf(", ");
    }
    printf("\n");
}

typedef struct i64ht{ 
    int32_t stepshift;int32_t capmask; 
    int32_t len; struct{int64_t key; cstring val;} *data; 
}i64ht;

void test_msi_i64(arena a) {
    i64ht *ht = (i64ht *)newmsi(&a, 5);
    typeof(ht->data) data = ht->data;

    printf("%lld\n", hash_i64(4));

    data[msi_set_by_ikey(ht, 5)].val = "Stamatto";
    data[msi_set_by_ikey(ht, 4)].val = "Sakamoto";
    data[msi_set_by_ikey(ht, 5)].val = "Ferreira";

    printf("pegou: %s\n", data[msi_get_by_ikey(ht, 5)].val);
    printf("não pegou: %s\n", data[msi_get_by_ikey(ht, 3)].val);

    for(int32_t i = 0; i < ht->capmask; ++i) {
        if  ( ! data[i].key) {
            continue;
        }
        printf("[%d]", i);
        printf("%lld: %s", data[i].key, data[i].val);
        printf(", ");
    }
    printf("\n");
}

threadlocal char bufferzao[8*MegaBytes];
int32_t main() {
    arena scratch = newarena(8*MegaBytes, malloc(8*MegaBytes));
    test_msi_i64(scratch);
    printf("\n");
    printf("\n");
    test_msi_cstr(scratch);

    return 0;
}
