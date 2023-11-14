#include <stdio.h>
#include "ale.h"

int32_t main(int32_t argc, const cstring *argv) {

    _ale_generic64 x = { .s="Alex" };

    printf("\n%lld %f %s %p\n", x.i, x.d, x.s, x.p);

    _ale_generic64 arr[5] = Zero;

    double f = 5.2;

    arr[0] = cpound(_ale_generic64) {.d = f}; 

    printf(" %f \n", arr[0].d);
}
