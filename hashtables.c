#include "ale.h"

void test_msi_upsert(arena a) {
    struct tables8s8{ lendata(struct {s8 key; s8 val;}); }
        ht = newmsi(&arena, ht, 1000);

    print("%llu", hash_it("Alessandro Stamatto"));

    msi_upsert(&ht, s8("Alessandro"), s8("Stamatto"));
    msi_upsert(&ht, s8("Sarah"), s8("Sakamoto"));
    msi_upsert(&ht, s8("Alessandro"), s8("Ferreira"));

    printf("%ld\n", msi_upsert(&ht, s8("sarah"), s8("sakamoto")));
    printf("%ld\n", msi_upsert(&ht, s8("alessandro"), (s8){0}));

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
    struct i64s8{ lendata(struct {i64 key; s8 val;}); }
        ht = newmsi(&arena, ht, 1000);

    print("%llu", hash_it((i64)4));

    msi_upsert(&ht, (i64)5, s8("Stamatto")); 
    msi_upsert(&ht, (i64)4, s8("Sakamoto"));
    msi_upsert(&ht, (i64)5, s8("Ferreira"));
    msi_upsert(&ht, (i64)5, (s8){0});

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
