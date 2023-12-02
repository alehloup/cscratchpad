#include <stdlib.h>
#include <stdio.h>
#include "ale.h"

_proc test_ht_str(arena_t scratch) {
    ht64_t ht = new_ht64(&scratch, 4);
    
    printf("%s hash: %lld\n", "Alessandro Stamatto", hash_str("Alessandro Stamatto"));

    hts_set_str(&ht, "Alessandro", "Stamatto");
    hts_set_str(&ht, "Sarah", "Sakamotto");
    hts_set_str(&ht, "Alessandro", "Ferreira");


    printf("Pegou: %s\n", hts_get_str(ht, "Sarah"));
    printf("Não pegou: %s\n", hts_get_str(ht, "Karol"));

    {
        entry_str_str *data = hts_data_as_str(&ht);
        for(i32 i = 0; i < ht.cap; ++i) {
            if  (! data[i].key) {
                continue;
            }
            printf("[%d]", i);
            printf("%s: %s", data[i].key, data[i].val);
            printf(", ");
        }
        printf("\n");
    }
}

_proc test_ht_i64(arena_t arena) {
    ht64_t ht = new_ht64(&arena, 4);

    printf("%d, hash: %lld\n", 4, hash_i64(4));

    hti_set_str(&ht, 5, "Stamatto");
    hti_set_str(&ht, 4, "Sakamoto");
    hti_set_str(&ht, 5, "Ferreira");

    printf("pegou: %s\n", hti_get_str(ht, 5));
    printf("não pegou: %s\n", hti_get_str(ht, 3));

    {
        entry_i64_str *data = hti_data_as_str(&ht);
        for(i32 i = 0; i < ht.cap; ++i) {
            if  ( ! data[i].key) {
                continue;
            }
            printf("[%d]", i);
            printf("%lld: %s", data[i].key, data[i].val);
            printf(", ");
        }
        printf("\n");
    }
}

_proc test_big_ht_i64(arena_t scratch) {
    ht64_t ht = new_ht64(&scratch, 128);

    printf("\n Big HT %d %d \n", ht.len, ht.cap);

    for(i32 i = 1; i < 128; ++i) {
        hti_set_i64(&ht, i, i);
    }

    {
        entry64_t *data = hti_data_as_int64(&ht);
        for(i32 i = 1; i <= ht.len; ++i) {
            i32 idx = hti_get_idx(ht, i);
            
            printf("[%d]", idx);
            printf("%lld: %lld", data[idx].key, data[idx].val);
            printf(", ");

            assert(data[idx].key == data[idx].val && "should be equal!");
        }
        for(i32 i = ht.len+1; i < ht.cap; ++i) {
            i32 idx = hti_get_idx(ht, i);

            assert(data[idx].key == 0 && "should be empty!");
        }
        printf("\n");   
    }
}

i32 main(void) {
    static u8 bufferzao[2*MBs_] = {0};
    arena_t scratch = newarena(sizeof(bufferzao), bufferzao);
    test_ht_i64(scratch);
    printf("\n");
    printf("\n");
    test_ht_str(scratch);
    printf("\n");
    printf("\n");
    test_big_ht_i64(scratch);

    return 0;
}
