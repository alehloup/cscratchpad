#include "ale.h"

typedef struct tables8s8{ lendata(struct {s8 key; s8 val;}); }tables8s8;

void test_msi_upsert(arena a) {
    tables8s8 ht = newmsi(&arena, ht, 1000);

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

int main() {

    arena scratch = newarena(_8MB);
    test_msi_upsert(scratch);

    return 0;
}
