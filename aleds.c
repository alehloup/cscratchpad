//#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "alearena.h"

tstruct(stat) {
    i64 x[10];
};
tstruct(dyna) {
    i8 *x;
};

int main() {
    stat s = {0};
    dyna d = {0};

    d.x = (i8 *)malloc(sizeof(*(d.x)) * 10);

    printf("%d %d", isstaticarr(s.x), isstaticarr(d.x));

    return 0;
}
