#include <stdio.h>
#include "ale.h"

_proc test_get_exp(void) {
    _new_array(hash_table, Long_Long, 1 << 21);
    printf("exp: %d", ht_get_exp(hash_table));
}

int main(void) {
    test_get_exp();

    return 0;
}