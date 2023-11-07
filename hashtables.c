#include "ale.h"

void test_msi_idx(arena a) {
    struct tabless{lendata(tkeyval(s8, s8));}
        ht = newmsi(&arena, ht, 1000);

    print("%llu", hash_it("Alessandro Stamatto"));

    msi_set(&ht, s("Alessandro"), s("Stamatto"));
    msi_set(&ht, s("Sarah"), s("Sakamoto"));
    msi_set(&ht, s("Alessandro"), s("Ferreira"));

    print("Pegou: %s", msi_get(&ht, s("Sarah")).data);
    print("Não pegou: %s", msi_get(&ht, s("Karol")).data);

    fori(msi_mask) {
        if (not ht.data[i].key.len) {
            continue;
        }
        printf("[%lld]", i);
        s8print(ht.data[i].key); printf(" "); s8print(ht.data[i].val);
        printf(", ");
    }
    printn;
}

void test_msi_idx2(arena a) {
    struct i64s{ lendata(tkeyval(i64, s8)); }
        ht = newmsi(&arena, ht, 1000);

    print("%llu", hash_it((i64)4));

    msi_set(&ht, 5, s("Stamatto")); 
    msi_set(&ht, 4, s("Sakamoto"));
    msi_set(&ht, 5, s("Ferreira"));

    typeof(ht) *pt = &ht;
    print("pegou: %s", msi_get(pt, 5).data);
    print("não pegou: %s", msi_get(pt, 3).data);

    fori(msi_mask) {
        if (not ht.data[i].key) {
            continue;
        }
        printf("[%lld]", i);
        printf("%lld: %s", ht.data[i].key, ht.data[i].val.data);
        printf(", ");
    }
    printn;
}

int main() {

    arena scratch = newarena(_8MB);
    test_msi_idx(scratch);
    printn;
    printn;
    test_msi_idx2(scratch);

    return 0;
}
