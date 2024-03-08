#include <stdio.h>
#include "ale.h"

typedef _structarray_type(int) array_int;
typedef _structarray_type(float) array_float;
typedef _structarray_type(double) array_double;

int main(void) {
    double array[256] = { 1.1, 2.2, 3.3 };
    array_double arr = _structarray_init(array, 3);

    printf("|%d| {%d} %f %f %f \n", arr.cap, arr.len, 
        (double) arr.array[0], (double) arr.array[1], (double) arr.array[2]);

    return 0;
}
