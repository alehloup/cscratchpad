#include "ale.h"

void test_msi_idx(arena a) {
    struct tables2s{msi_ht_data(s8, s8);}
        ht = newmsi(&a, ht, 1000);

    print("%llu", hash_it("Alessandro Stamatto"));

    msi_set(&ht, s("Alessandro"), s("Stamatto"));
    msi_set(&ht, s("Sarah"), s("Sakamoto"));
    msi_set(&ht, s("Alessandro"), s("Ferreira"));

    print("Pegou: %s", msi_get(&ht, s("Sarah")).data);
    print("Não pegou: %s", msi_get(&ht, s("Karol")).data);

    for(int i = 0; i < ht.capmask; ++i) {
        if  ( ! ht.data[i].key.len) {
            continue;
        }
        printf("[%d]", i);
        print_s8(ht.data[i].key); printf(" "); print_s8(ht.data[i].val);
        printf(", ");
    }
    printn;
}

void test_msi_idx2(arena a) {
    struct tablei2s{msi_ht_data(long long, s8);}
        ht = newmsi(&a, ht, 1000);

    print("%llu", hash_it((long long)4));

    msi_set(&ht, 5, s("Stamatto")); 
    msi_set(&ht, 4, s("Sakamoto"));
    msi_set(&ht, 5, s("Ferreira"));

    typeof(ht) *pt = &ht;
    print("pegou: %s", msi_get(pt, 5).data);
    print("não pegou: %s", msi_get(pt, 3).data);

    for(int i = 0; i < ht.capmask; ++i) {
        if  ( ! ht.data[i].key) {
            continue;
        }
        printf("[%d]", i);
        printf("%lld: %s", ht.data[i].key, ht.data[i].val.data);
        printf(", ");
    }
    printn;
}

void test_msi_idx3(arena scratch) {
    struct tab{ msi_ht_data(s8, int); } 
        ht = newmsi(&scratch, ht, 2000);

    msi_set(&ht, s("Alex"), msi_current_val + 1);
    msi_set(&ht, s("Alex"), msi_current_val + 51);
    msi_set(&ht, s("Sarah"), msi_current_val + 4);
    msi_set(&ht, s("Sarah"), msi_current_val * 13);

    for(int i = 0; i < ht.capmask; ++i) {
        if  ( ! ht.data[i].key.len) {
            continue;
        }
        printf("[%d]", i);
        printf("%s: %d", ht.data[i].key.data, ht.data[i].val);
        printf(", ");
    }
    printn;
}

threadlocal u8 bufferzao[8*MegaBytes];
int main() {
    arena scratch = newarena(8*MegaBytes, malloc(8*MegaBytes));
    test_msi_idx(scratch);
    printn;
    printn;
    test_msi_idx2(scratch);

    test_msi_idx3(scratch);

    return 0;
}
