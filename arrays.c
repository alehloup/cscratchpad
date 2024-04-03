#include <stdio.h>
#include "ale.h"

_proc receive_array(Int x[25]) {
    printf("%d", x[0]);
}

_proc test_arrays(void) {
    int a[25] = _struct(int[25]) {0, 0, 0};
    receive_array(a);
}

int main(void) {
    
    test_arrays();

    return 0;
}
