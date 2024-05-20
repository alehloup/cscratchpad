#include "ale.h"

proc_ test_get_exp(void) {
    int hash_table[1 << 6] = {0};
    printf("exp: %u\n", highbit((unsigned int)arrsizeof(hash_table)));
}

proc_ test_hashtable_insertions(void) {
    unsigned int pos = 0;
    struct lenstr_t keys[1 << 6] = {{0, 0}};
    size_t keys_len = 0;

    printf("Keys cap: %zu\n", arrsizeof(keys));

    pos = ht_lenstr_upsert(to_lenstr("Alessandro Stamatto"), arrsizeof(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].str);

    pos = ht_lenstr_upsert(to_lenstr("Sarah Sakamoto"), arrsizeof(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].str);

    pos = ht_lenstr_upsert(to_lenstr("Adir Luiz"), arrsizeof(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].str);

    pos = ht_lenstr_upsert(to_lenstr("Ines Stamatto"), arrsizeof(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].str);

    pos = ht_lenstr_upsert(to_lenstr("Luane Stamatto"), arrsizeof(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].str);

    pos = ht_lenstr_upsert(to_lenstr("Alessandro Stamatto"), arrsizeof(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].str);

    pos = ht_lenstr_upsert(to_lenstr("Luane Stam"), arrsizeof(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].str);

    pos = ht_lenstr_upsert(to_lenstr("Sarah Sakamoto"), arrsizeof(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].str);

    pos = ht_lenstr_upsert(to_lenstr("Alex Stam"), arrsizeof(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].str);

    pos = ht_lenstr_upsert(to_lenstr("Alessandro Stam"), arrsizeof(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].str);

    printf("Len: %zu Cap: %zu\n", keys_len, arrsizeof(keys));
}


int main(void) {
    test_get_exp();
    test_hashtable_insertions();

    printf("\nTested hashtable functions.\n");

    return 0;
}
