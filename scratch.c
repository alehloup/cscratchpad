#include "ale.h"

static uint8_t buffer[2*_Mega_Bytes];

$fun int teste(int x) {
    return (x+1)*2;
}

$fun int teste2(int v[1]) {
    return *v * 3;
}

int main(void) {
    int x = 5;
    printf("%d", teste2(&x));
    return 0;
}
