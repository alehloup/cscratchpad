#include <stdlib.h>
#include "ale.h"

typedef struct strht{ 
    int32_t stepshift;int32_t capmask; 
    int32_t len; struct{s8 key; s8 val;} *data; 
}strht;

void test_msi_idx(arena a) {
    strht ht = newmsi(&a, ht, 1000);

    printf("%lld\n", hash_it("Alessandro Stamatto"));

    msi_set(&ht, s("Alessandro"), s("Stamatto"));
    msi_set(&ht, s("Sarah"), s("Sakamoto"));
    msi_set(&ht, s("Alessandro"), s("Ferreira"));

    printf("Pegou: %s\n", msi_get(&ht, s("Sarah")).data);
    printf("Não pegou: %s\n", msi_get(&ht, s("Karol")).data);

    for(int32_t i = 0; i < ht.capmask; ++i) {
        if  ( ! ht.data[i].key.len) {
            continue;
        }
        printf("[%d]", i);
        print_s8(ht.data[i].key); printf(" "); print_s8(ht.data[i].val);
        printf(", ");
    }
    printf("\n");
}

typedef struct i64ht{ 
    int32_t stepshift;int32_t capmask; 
    int32_t len; struct{int64_t key; s8 val;} *data; 
}i64ht;

void test_msi_idx2(arena a) {
    i64ht ht = newmsi(&a, ht, 1000);

    printf("%lld\n", hash_it(4));

    msi_set(&ht, 5, s("Stamatto")); 
    msi_set(&ht, 4, s("Sakamoto"));
    msi_set(&ht, 5, s("Ferreira"));

    typeof(ht) *pt = &ht;
    printf("pegou: %s\n", msi_get(pt, 5).data);
    printf("não pegou: %s\n", msi_get(pt, 3).data);

    for(int32_t i = 0; i < ht.capmask; ++i) {
        if  ( ! ht.data[i].key) {
            continue;
        }
        printf("[%d]", i);
        printf("%lld: %s", ht.data[i].key, ht.data[i].val.data);
        printf(", ");
    }
    printf("\n");
}

static threadlocal char bufferzao[8*MegaBytes];
int32_t main() {
    arena scratch = newarena(8*MegaBytes, malloc(8*MegaBytes));
    test_msi_idx(scratch);
    printf("\n");
    printf("\n");
    test_msi_idx2(scratch);

    return 0;
}
