#include "ale.h"

proc_ test_get_exp(void) {
    arrnew_(hash_table, int64_t, 1 << 21);
    printf("exp: %d\n", array_cap_to_exp(hash_table_cap));
}

proc_ test_keys(void) {
    arrnew_(keys, int64_t, 1 << 10);
    printf("cap: %lld %d\n", keys_cap, array_cap_to_exp(keys_cap));
}

proc_ test_hashtable_insertions(void) {
    int32_t pos = 0;
    arrnew_(keys, struct sslice_t, 1 << 10);

    printf("Keys cap: %lld\n", keys_cap);

    pos = sslice_msi_upsert(sslice_("Alessandro Stamatto"), arrarg_(keys));
    printf("[%d] %s \n", pos, keys[pos].text);

    pos = sslice_msi_upsert(sslice_("Sarah Sakamoto"), arrarg_(keys));
    printf("[%d] %s \n", pos, keys[pos].text);

    pos = sslice_msi_upsert(sslice_("Adir Luiz"), arrarg_(keys));
    printf("[%d] %s \n", pos, keys[pos].text);

    pos = sslice_msi_upsert(sslice_("Ines Stamatto"), arrarg_(keys));
    printf("[%d] %s \n", pos, keys[pos].text);

    pos = sslice_msi_upsert(sslice_("Luane Stamatto"), arrarg_(keys));
    printf("[%d] %s \n", pos, keys[pos].text);

    pos = sslice_msi_upsert(sslice_("Alessandro Stamatto"), arrarg_(keys));
    printf("[%d] %s \n", pos, keys[pos].text);

    pos = sslice_msi_upsert(sslice_("Luane Stam"), arrarg_(keys));
    printf("[%d] %s \n", pos, keys[pos].text);

    pos = sslice_msi_upsert(sslice_("Sarah Sakamoto"), arrarg_(keys));
    printf("[%d] %s \n", pos, keys[pos].text);

    pos = sslice_msi_upsert(sslice_("Alex Stam"), arrarg_(keys));
    printf("[%d] %s \n", pos, keys[pos].text);

    pos = sslice_msi_upsert(sslice_("Alessandro Stam"), arrarg_(keys));
    printf("[%d] %s \n", pos, keys[pos].text);

    printf("Len: %lld Cap: %lld\n", (*keys_len), keys_cap);
}


int32_t main(void) {
    test_get_exp();
    test_keys();
    test_hashtable_insertions();

    printf("\nTested hashtable functions.\n");

    return 0;
}
