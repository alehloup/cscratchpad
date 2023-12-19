#include <stdio.h>
#include "ale.h"

_proc test_strset(void) {
    static cstr keys[HT_CAP] = {0};
    len32 keys_len = 0;

    printf("Insert %s = %d (len: %d)\n", "Alex", str_in_ht_("Alex", keys, &keys_len), keys_len);
    printf("Insert %s = %d (len: %d)\n", "Sarah", str_in_ht_("Sarah", keys, &keys_len), keys_len);
    printf("Insert %s = %d (len: %d)\n", "Alex", str_in_ht_("Alex", keys, &keys_len), keys_len);
    printf("Insert %s = %d (len: %d)\n", "Alex", str_in_ht_("Alex", keys, &keys_len), keys_len);

    printf("Get %s = %d (len: %d)\n", "Sarah", str_in_ht_("Sarah", keys, 0), keys_len);
    printf("Get %s = %d (len: %d)\n", "Karol", str_in_ht_("Karol", keys, 0), keys_len);
    printf("Get %s = %d (len: %d)\n", "Isabelle", str_in_ht_("Isabelle", keys, 0), keys_len);
}

int main(void) {
    test_strset();

    return 0;
}
