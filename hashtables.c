#include <stdlib.h>
#include <stdio.h>
#include "ale.h"

typedef struct strht{ 
     int32_t shift;int32_t mask; 
    int32_t len; struct{cstring key; cstring val;} *data; 
}strht;

void test_msi_cstr(arena a) {
    strht *ht = (strht *)new_64msi(&a, 4);
    typeof(ht->data) data = ht->data;
    
    printf("%lld\n", hash_cstr("Alessandro Stamatto"));

    data[msi_set_skey(ht, "Alessandro")].val = "Stamatto";
    data[msi_set_skey(ht, "Sarah")].val = "Sakamotto";
    data[msi_set_skey(ht, "Alessandro")].val = "Ferreira";

    printf("Pegou: %s\n", data[msi_get_by_skey(ht, "Sarah")].val);
    printf("Não pegou: %s\n", data[msi_get_by_skey(ht, "Karol")].val);

    for(int32_t i = 0; i < ht->mask; ++i) {
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
    int32_t shift;int32_t mask; 
    int32_t len; struct{int64_t key; cstring val;} *data; 
}i64ht;

void test_msi_i64(arena a) {
    i64ht *ht = (i64ht *)new_64msi(&a, 4);
    typeof(ht->data) data = ht->data;

    printf("%lld\n", hash_i64(4));

    data[msi_set_i64key(ht, 5)].val = "Stamatto";
    data[msi_set_i64key(ht, 4)].val = "Sakamoto";
    data[msi_set_i64key(ht, 5)].val = "Ferreira";

    printf("pegou: %s\n", data[msi_get_by_i64key(ht, 5)].val);
    printf("não pegou: %s\n", data[msi_get_by_i64key(ht, 3)].val);

    for(int32_t i = 0; i < ht->mask; ++i) {
        if  ( ! data[i].key) {
            continue;
        }
        printf("[%d]", i);
        printf("%lld: %s", data[i].key, data[i].val);
        printf(", ");
    }
    printf("\n");
}

typedef struct i64bight{ 
    int32_t shift;int32_t mask; 
    int32_t len; struct{int64_t key; int64_t val;} *data; 
}i64bight;

void test_big_msi_i64(arena a) {
    i64bight *ht = (i64bight *)new_64msi(&a, 128);
    typeof(ht->data) data = ht->data;

    for(int i = 0; i < 128; ++i) {
        data[msi_set_i64key(ht, i)].val = i;
    }

    for(int32_t i = 0; i < ht->len; ++i) {
        int32_t idx = msi_get_by_i64key(ht, i);
        
        printf("[%d]", idx);
        printf("%lld: %lld", data[idx].key, data[idx].val);
        printf(", ");

        ale_assert(data[idx].key == data[idx].val, "should be equal!");
    }

    for(int32_t i = ht->len; i < ht->mask; ++i) {
        int32_t idx = msi_get_by_i64key(ht, i);

        ale_assert(data[idx].key == 0, "should be empty!");
    }


    printf("\n");
}

threadlocal char bufferzao[2*_Mega_Bytes];
int32_t main() {
    arena scratch = newarena(sizeof(bufferzao), bufferzao);
    test_msi_i64(scratch);
    printf("\n");
    printf("\n");
    test_msi_cstr(scratch);
    printf("\n");
    printf("\n");
    test_big_msi_i64(scratch);

    return 0;
}
