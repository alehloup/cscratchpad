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

int main(void) {
    test_get_exp();

    test_keys();

    return 0;
}