#include "./aleh/string.h"
#include "./aleh/htmsi.h"

enum { table_cap = (1u << 6u) };

static inline void test_hashtable_insertions(void) {
    unsigned int pos = 0;
    
    struct lenstr_t keys[table_cap] = {{0, 0}};
    unsigned int keys_len = 0;

    printf("Keys cap: %d\n", table_cap);

    pos = ht_msi_lenstr_upsert(to_lenstr("Alessandro Stamatto"), table_cap, keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].str);

    pos = ht_msi_lenstr_upsert(to_lenstr("Sarah Sakamoto"), table_cap, keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].str);

    pos = ht_msi_lenstr_upsert(to_lenstr("Adir Luiz"), table_cap, keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].str);

    pos = ht_msi_lenstr_upsert(to_lenstr("Ines Stamatto"), table_cap, keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].str);

    pos = ht_msi_lenstr_upsert(to_lenstr("Luane Stamatto"), table_cap, keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].str);

    pos = ht_msi_lenstr_upsert(to_lenstr("Alessandro Stamatto"), table_cap, keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].str);

    pos = ht_msi_lenstr_upsert(to_lenstr("Luane Stam"), table_cap, keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].str);

    pos = ht_msi_lenstr_upsert(to_lenstr("Sarah Sakamoto"), table_cap, keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].str);

    pos = ht_msi_lenstr_upsert(to_lenstr("Alex Stam"), table_cap, keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].str);

    pos = ht_msi_lenstr_upsert(to_lenstr("Alessandro Stam"), table_cap, keys, &keys_len);
    printf("[%u] %s \n", pos, keys[pos].str);

    printf("Len: %u Cap: %d\n", keys_len, table_cap);
}


int main(void) {
    test_hashtable_insertions();

    printf("\nTested hashtable functions.\n");

    return 0;
}
