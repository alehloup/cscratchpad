#include "ale.h"

static inline void test_get_exp(void) {
    int hash_table[1 << 6] = {0};
    printf("exp: %u\n", highbit((unsigned int)(unsigned int)arrsizeof(hash_table)));
}

static inline void test_hashtable_insertions(void) {
    unsigned int pos = 0;
    struct lenstr_t keys[1 << 6] = {{0, 0}};
    unsigned int keys_len = 0;

    printf("Keys cap: %u\n", (unsigned int)arrsizeof(keys));

    pos = ht_lenstr_upsert(to_lenstr("Alessandro Stamatto"), (unsigned int)arrsizeof(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].str);

    pos = ht_lenstr_upsert(to_lenstr("Sarah Sakamoto"), (unsigned int)arrsizeof(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].str);

    pos = ht_lenstr_upsert(to_lenstr("Adir Luiz"), (unsigned int)arrsizeof(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].str);

    pos = ht_lenstr_upsert(to_lenstr("Ines Stamatto"), (unsigned int)arrsizeof(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].str);

    pos = ht_lenstr_upsert(to_lenstr("Luane Stamatto"), (unsigned int)arrsizeof(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].str);

    pos = ht_lenstr_upsert(to_lenstr("Alessandro Stamatto"), (unsigned int)arrsizeof(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].str);

    pos = ht_lenstr_upsert(to_lenstr("Luane Stam"), (unsigned int)arrsizeof(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].str);

    pos = ht_lenstr_upsert(to_lenstr("Sarah Sakamoto"), (unsigned int)arrsizeof(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].str);

    pos = ht_lenstr_upsert(to_lenstr("Alex Stam"), (unsigned int)arrsizeof(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].str);

    pos = ht_lenstr_upsert(to_lenstr("Alessandro Stam"), (unsigned int)arrsizeof(keys), keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].str);

    printf("Len: %u Cap: %u\n", keys_len, (unsigned int)arrsizeof(keys));
}


int main(void) {
    test_get_exp();
    test_hashtable_insertions();

    printf("\nTested hashtable functions.\n");

    return 0;
}
