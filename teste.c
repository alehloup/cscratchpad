#include <stdio.h>
#include "ale.h"

int main(void) {
    int x = 52696952;
    hash64 hn = hash_int(x);
    printf("x:%d hash:%llu\n", x, hn);
    printf("x:%d hash:%llu\n", 52, hash_int(52));

    ccstr y = "Alex";
    hash64 hs = hash_str(y);
    printf("y:%s hash:%llu\n", y, hs);
    printf("y:%s hash:%llu\n", "Sarah", hash_str("Sarah"));

    return 0;
}
