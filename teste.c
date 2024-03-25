#include <stdio.h>
#include "ale.h"

int main(void) {
    Double array[256] = { 1.1, 2.2, 3.3 };
    Array_double arr = A(array, 3);

    _append(&arr, 5.2);

    for(int i = 0; i < arr.len; ++i) {
        printf("%f ", arr.data[i]);
    }
    printf("\n");

    _delidx(&arr, 1);

    for(int i = 0; i < arr.len; ++i) {
        printf("%f ", arr.data[i]);
    }
    printf("\n");

    String s = String("Testando strings!");
    printf("{%lld} %s \n", s.len, s.data);

    return 0;
}
