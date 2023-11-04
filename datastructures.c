//#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "ale.h"

typedef struct at{
    ARR_HEADER_DATA(i64);
}at;
void test_push(arena scratch) {
    at d = {};

    fori(164) {
        *push(&d, &scratch) = i;
    }

    print_arr("Vetor dinâmico", d, "%lld");
}

typedef struct table{
    HT_HEADER_DATA(i64, b32);
}table;
void test_ht(arena a) {
    table *ht = newht(&a, table, 256);
    print_ht("Hash Table", (*ht), "%lld %ld");
}

int main() {

    arena scratch = newarena(_8MB);
    test_ht(scratch);

    return 0;
}
