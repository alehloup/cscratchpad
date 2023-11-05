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
    HT_HEADER_DATA(s8, s8);
}table;
void test_ht(arena a) {
    table *ht = newht(&a, table, 256);

    ht->data[5].key = s8("Teste Chave");
    ht->data[5].val = s8("Teste Valor");

    ht->data[82].key = s8("Teste Chave 82");

    ht_foridx_print(el, "Hash Table", (*ht), 
        s8print(el.key);
        printf(":");
        s8print(el.val)
    );
}

int main() {

    arena scratch = newarena(_8MB);
    test_ht(scratch);

    return 0;
}
