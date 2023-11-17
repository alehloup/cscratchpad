#include <stdio.h>
#include "ale.h"

int32_t main(int32_t argc, const cstring *argv) {

    _ale_generic64 x = { .s="Alex" };

    ale_printf("\n%lld %f %s %p\n", x.i, x.d, x.s, x.p);

    int64_t ints[5] = {1, 2, 3, 4, 5};

    int64_t *arr = ints;

    float64_t f = 5.2;

    float64_t *f64s = (float64_t *) ints;

    //memcpy(&f64s, &arr, 8);

    f64s[0] = f;

    ale_printf(" %f \n", f64s[0]);
}
