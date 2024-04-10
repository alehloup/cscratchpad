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

_proc test_new_hashtable(void) {
    _new_htable(ht, String, String, 8);
    printf("%lld %lld %lld \n", ht.keys.cap, ht.values.cap, arraysizeof(htkeys_base));
}

_fun Long test_static_array(void) {
    _new_array(vetor, static thread_local Int, 16);

    return vetor.len;
}

int main(void) {
    test_get_exp();
    test_keys();
    test_new_hashtable();

    return 0;
}
