#include <stdio.h>
#include "ale.h"

int32_t main(int32_t argc, const cstring *argv) {

    _ale_generic64 x = { .s="Alex" };

    ale_printf("\n%lld %f %s %p\n", x.i, x.d, x.s, x.p);

    int64_t ints[5] = {1, 2, 3, 4, 5};

    int64_t *arr = ints;

    double f = 5.2;

    double *doubles = (double *) ints;

    //memcpy(&doubles, &arr, 8);

    doubles[0] = f;

    ale_printf(" %f \n", doubles[0]);
}
