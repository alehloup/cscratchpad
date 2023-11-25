#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void print_mem(void * ptr, int32_t len) {
    uint8_t * mem = (uint8_t *) ptr;

    printf("\n");
    printf("[%lld] ", (int64_t) ptr);
    for (int i = 0; i < len; ++i) {
        if (i > 0 && i % 8 == 0) {
            printf(" ;; ");
        }
        printf("|%d| ", mem[i]);
    }
    printf("\n");
}

int main(void) {
    int64_t* arr = (int64_t *) malloc(24);
    uint8_t * fill = (uint8_t *) arr;
    for (uint8_t i = 0; i < 24; ++i) {
        fill[i] = i + 200;
    }

    arr[0] = (int64_t) "String 1";
    arr[1] = (int64_t) "String 2";

    print_mem(&arr[0], 8);
    print_mem(&arr[1], 8);
    print_mem(&arr[2], 8);
    print_mem(arr, 24);

    char* *strs = 0;
    strs = (char* *) arr;

    printf("\n%s, %s\n", strs[0], strs[1]);
    // GCC/Clang: String 1, String 2, String 3
    // MSVC cl: String 1, (null), String 2

    print_mem(&strs[0], 8);
    print_mem(&strs[1], 8);
    print_mem(&strs[2], 8);
    print_mem(strs, 24);

    return 0;
}
