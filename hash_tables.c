#include <stdio.h>
#include "ale.h"

_proc test_get_exp(void) {
    _new_array(hash_table, Long, 1 << 21);
    printf("exp: %d\n", array_cap_to_exp(hash_table.cap));
}

_proc test_keys(void) {
    _new_array(keys, Long, 1 << 10);
    printf("cap: %lld %d\n", keys.cap, array_cap_to_exp(keys.cap));
}

_proc test_hashtable_insertions(void) {
    Int pos = 0;
    _new_array(keys, String, 1 << 10);

    printf("Keys cap: %lld\n", keys.cap);

    pos = strings_msi_upsert(S("Alessandro Stamatto"), &keys);
    printf("[%d] %s \n", pos, keys.elements[pos].text);

    pos = strings_msi_upsert(S("Sarah Sakamoto"), &keys);
    printf("[%d] %s \n", pos, keys.elements[pos].text);

    pos = strings_msi_upsert(S("Adir Luiz"), &keys);
    printf("[%d] %s \n", pos, keys.elements[pos].text);

    pos = strings_msi_upsert(S("Ines Stamatto"), &keys);
    printf("[%d] %s \n", pos, keys.elements[pos].text);

    pos = strings_msi_upsert(S("Luane Stamatto"), &keys);
    printf("[%d] %s \n", pos, keys.elements[pos].text);

    pos = strings_msi_upsert(S("Alessandro Stamatto"), &keys);
    printf("[%d] %s \n", pos, keys.elements[pos].text);

    pos = strings_msi_upsert(S("Luane Stam"), &keys);
    printf("[%d] %s \n", pos, keys.elements[pos].text);

    pos = strings_msi_upsert(S("Sarah Sakamoto"), &keys);
    printf("[%d] %s \n", pos, keys.elements[pos].text);

    pos = strings_msi_upsert(S("Alex Stam"), &keys);
    printf("[%d] %s \n", pos, keys.elements[pos].text);

    pos = strings_msi_upsert(S("Alessandro Stam"), &keys);
    printf("[%d] %s \n", pos, keys.elements[pos].text);

    printf("Len: %lld Cap: %lld\n", keys.len, keys.cap);
}


int main(void) {
    test_get_exp();
    test_keys();
    test_hashtable_insertions();

    return 0;
}
