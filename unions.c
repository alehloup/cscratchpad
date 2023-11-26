#include <stdio.h>
#include "ale.h"

typedef union union64_t{
    int64_t i;
    float64_t d;
    cstr_t s;
    void * p;
}union64_t;
typedef union union32_t{
    int32_t i;
    float32_t d;
}union32_t;

int32_t main() {

    union64_t x = { .s="Alex" };

    printf("\n%lld %f %s %p\n", x.i, x.d, x.s, x.p);

    int64_t ints[5] = {1, 2, 3, 4, 5};

    float64_t f = 5.2;

    float64_t *f64s = (float64_t *) ints;

    //memcpy(&f64s, &arr, 8);

    f64s[0] = f;

    printf(" %f \n", f64s[0]);
}
