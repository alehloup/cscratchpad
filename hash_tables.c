#include "ale.h"

proc_ test_get_exp(void) {
    int hash_table[1 << 6] = {0};
    printf("exp: %u\n", highbit((unsigned int)arrsizeof(hash_table)));
}

proc_ test_hashtable_insertions(void) {
    unsigned int pos = 0;
    struct sslice_t keys[1 << 6] = {{0, 0}};
    size_t keys_len = 0;

    printf("Keys cap: %zu\n", arrsizeof(keys));

    pos = ht_sslice_upsert(to_sslice("Alessandro Stamatto"), arrsizeof(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].text);

    pos = ht_sslice_upsert(to_sslice("Sarah Sakamoto"), arrsizeof(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].text);

    pos = ht_sslice_upsert(to_sslice("Adir Luiz"), arrsizeof(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].text);

    pos = ht_sslice_upsert(to_sslice("Ines Stamatto"), arrsizeof(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].text);

    pos = ht_sslice_upsert(to_sslice("Luane Stamatto"), arrsizeof(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].text);

    pos = ht_sslice_upsert(to_sslice("Alessandro Stamatto"), arrsizeof(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].text);

    pos = ht_sslice_upsert(to_sslice("Luane Stam"), arrsizeof(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].text);

    pos = ht_sslice_upsert(to_sslice("Sarah Sakamoto"), arrsizeof(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].text);

    pos = ht_sslice_upsert(to_sslice("Alex Stam"), arrsizeof(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].text);

    pos = ht_sslice_upsert(to_sslice("Alessandro Stam"), arrsizeof(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].text);

    printf("Len: %zu Cap: %zu\n", keys_len, arrsizeof(keys));
}


int main(void) {
    test_get_exp();
    test_hashtable_insertions();

    printf("\nTested hashtable functions.\n");

    return 0;
}
