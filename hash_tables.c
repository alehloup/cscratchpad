#include <stdio.h>
#include "ale.h"

_proc test_get_exp(void) {
    _new_array(hash_table, Long, 1 << 21);
    printf("exp: %d", array_cap_to_exp(hash_table.cap));
}

int main(void) {
    test_get_exp();

    return 0;
}