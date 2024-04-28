#include "ale.h"

proc_ test_get_exp(void) {
    int32_t hash_table[1 << 6] = ZERO_INIT_;
    printf("exp: %d\n", array_cap_to_exp(ARRCAP_(hash_table)));
}

proc_ test_hashtable_insertions(void) {
    int32_t pos = 0;
    struct sslice_t keys[1 << 6] = ZERO_INIT_;
    int64_t keys_len = 0;

    printf("Keys cap: %" PRId64 "\n", ARRCAP_(keys));

    pos = ht_sslice_upsert(to_sslice("Alessandro Stamatto"), ARRCAP_(keys), keys, &keys_len);
    printf("[%d] %s \n", pos, keys[pos].text);

    pos = ht_sslice_upsert(to_sslice("Sarah Sakamoto"), ARRCAP_(keys), keys, &keys_len);
    printf("[%d] %s \n", pos, keys[pos].text);

    pos = ht_sslice_upsert(to_sslice("Adir Luiz"), ARRCAP_(keys), keys, &keys_len);
    printf("[%d] %s \n", pos, keys[pos].text);

    pos = ht_sslice_upsert(to_sslice("Ines Stamatto"), ARRCAP_(keys), keys, &keys_len);
    printf("[%d] %s \n", pos, keys[pos].text);

    pos = ht_sslice_upsert(to_sslice("Luane Stamatto"), ARRCAP_(keys), keys, &keys_len);
    printf("[%d] %s \n", pos, keys[pos].text);

    pos = ht_sslice_upsert(to_sslice("Alessandro Stamatto"), ARRCAP_(keys), keys, &keys_len);
    printf("[%d] %s \n", pos, keys[pos].text);

    pos = ht_sslice_upsert(to_sslice("Luane Stam"), ARRCAP_(keys), keys, &keys_len);
    printf("[%d] %s \n", pos, keys[pos].text);

    pos = ht_sslice_upsert(to_sslice("Sarah Sakamoto"), ARRCAP_(keys), keys, &keys_len);
    printf("[%d] %s \n", pos, keys[pos].text);

    pos = ht_sslice_upsert(to_sslice("Alex Stam"), ARRCAP_(keys), keys, &keys_len);
    printf("[%d] %s \n", pos, keys[pos].text);

    pos = ht_sslice_upsert(to_sslice("Alessandro Stam"), ARRCAP_(keys), keys, &keys_len);
    printf("[%d] %s \n", pos, keys[pos].text);

    printf("Len: %" PRId64 " Cap: %" PRId64 "\n", keys_len, ARRCAP_(keys));
}


int32_t main(void) {
    test_get_exp();
    test_hashtable_insertions();

    printf("\nTested hashtable functions.\n");

    return 0;
}
