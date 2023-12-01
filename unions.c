#include <stdio.h>
#include "ale.h"

typedef union union64_t{
    i64 i;
    f64 d;
    cstr s;
    void * p;
}union64_t;
typedef union union32_t{
    i32 i;
    f32 d;
}union32_t;

i32 main(void) {

    union64_t x = { .s="Alex" };

    printf("\n%lld %f %s %p\n", x.i, x.d, x.s, x.p);

    i64 ints[5] = {1, 2, 3, 4, 5};

    f64 f = 5.2;

    f64 *f64s = (f64 *) ints;

    //memcpy(&f64s, &arr, 8);

    f64s[0] = f;

    printf(" %f \n", f64s[0]);
}
