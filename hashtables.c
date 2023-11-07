#include "ale.h"

void test_msi_upsert(arena a) {
    struct tabless{ lendata(struct {s8 key; s8 val;}); }
        ht = newmsi(&arena, ht, 1000);

    print("%llu", hash_it("Alessandro Stamatto"));

    msi_upsert(&ht, s("Alessandro"), s("Stamatto"));
    msi_upsert(&ht, s("Sarah"), s("Sakamoto"));
    msi_upsert(&ht, s("Alessandro"), s("Ferreira"));

    printf("%ld\n", msi_upsert(&ht, s("sarah"), s("sakamoto")));
    printf("%ld\n", msi_upsert(&ht, s("Alessandro"), msi_only_get));

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

void test_msi_upsert2(arena a) {
    struct i64s{ lendata(struct {i64 key; s8 val;}); }
        ht = newmsi(&arena, ht, 1000);

    print("%llu", hash_it((i64)4));

    msi_upsert(&ht, 5, s("Stamatto")); 
    msi_upsert(&ht, 4, s("Sakamoto"));
    msi_upsert(&ht, 5, s("Ferreira"));

    typeof(ht) *pt = &ht;
    print("pegou: %s", msi_get(*pt, 5).data);

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
    test_msi_upsert(scratch);
    printn;
    printn;
    test_msi_upsert2(scratch);

    return 0;
}
