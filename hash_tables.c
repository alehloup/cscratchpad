#include "ale.h"

proc_ test_get_exp(void) {
    int hash_table[1 << 6] = ZERO_INIT_;
    printf("exp: %u\n", array_cap_to_exp(CAP_(hash_table)));
}

proc_ test_hashtable_insertions(void) {
    unsigned int pos = 0;
    struct sslice_t keys[1 << 6] = ZERO_INIT_;
    size_t keys_len = 0;

    printf("Keys cap: %zu\n", CAP_(keys));

    pos = ht_sslice_upsert(to_sslice("Alessandro Stamatto"), CAP_(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].text);

    pos = ht_sslice_upsert(to_sslice("Sarah Sakamoto"), CAP_(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].text);

    pos = ht_sslice_upsert(to_sslice("Adir Luiz"), CAP_(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].text);

    pos = ht_sslice_upsert(to_sslice("Ines Stamatto"), CAP_(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].text);

    pos = ht_sslice_upsert(to_sslice("Luane Stamatto"), CAP_(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].text);

    pos = ht_sslice_upsert(to_sslice("Alessandro Stamatto"), CAP_(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].text);

    pos = ht_sslice_upsert(to_sslice("Luane Stam"), CAP_(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].text);

    pos = ht_sslice_upsert(to_sslice("Sarah Sakamoto"), CAP_(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].text);

    pos = ht_sslice_upsert(to_sslice("Alex Stam"), CAP_(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].text);

    pos = ht_sslice_upsert(to_sslice("Alessandro Stam"), CAP_(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].text);

    printf("Len: %zu Cap: %zu\n", keys_len, CAP_(keys));
}


int main(void) {
    test_get_exp();
    test_hashtable_insertions();

    printf("\nTested hashtable functions.\n");

    return 0;
}
