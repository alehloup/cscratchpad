#include <stdlib.h>
#include "ale.h"

typedef struct strht{ 
    int32_t stepshift;int32_t capmask; 
    int32_t len; struct{cstring key; cstring val;} *data; 
}strht;

void test_msi_idx(arena a) {
    strht *ht = (strht *)newmsi(&a, 1000);

    printf("%lld\n", hash_it("Alessandro Stamatto"));

    msi_set(ht, "Alessandro", "Stamatto");
    msi_set(ht, "Sarah", "Sakamoto");
    msi_set(ht, "Alessandro", "Ferreira");

    printf("Pegou: %s\n", msi_get(ht, "Sarah"));
    printf("Não pegou: %s\n", msi_get(ht, "Karol"));

    for(int32_t i = 0; i < ht->capmask; ++i) {
        if  ( ! ht->data[i].key) {
            continue;
        }
        printf("[%d]", i);
        printf("%s:%s ", ht->data[i].key, ht->data[i].val);
        printf(", ");
    }
    printf("\n");
}

typedef struct i64ht{ 
    int32_t stepshift;int32_t capmask; 
    int32_t len; struct{int64_t key; cstring val;} *data; 
}i64ht;

void test_msi_idx2(arena a) {
    i64ht *ht = (i64ht *)newmsi(&a, 1000);

    printf("%lld\n", hash_it(4));

    msi_set(ht, 5, "Stamatto"); 
    msi_set(ht, 4, "Sakamoto");
    msi_set(ht, 5, "Ferreira");

    printf("pegou: %s\n", msi_get(ht, 5));
    printf("não pegou: %s\n", msi_get(ht, 3));

    for(int32_t i = 0; i < ht->capmask; ++i) {
        if  ( ! ht->data[i].key) {
            continue;
        }
        printf("[%d]", i);
        printf("%lld: %s", ht->data[i].key, ht->data[i].val);
        printf(", ");
    }
    printf("\n");
}

threadlocal char bufferzao[8*MegaBytes];
int32_t main() {
    arena scratch = newarena(8*MegaBytes, malloc(8*MegaBytes));
    //test_msi_idx(scratch);
    printf("\n");
    printf("\n");
    test_msi_idx2(scratch);

    hash_it("Woa!");
    cstring blah = "Blah!";
    hash_it(blah);

    return 0;
}
