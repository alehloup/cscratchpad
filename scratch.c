#include "ale.h"

$fun int teste(int x) {
    return (x+1)*2;
}

$fun int teste2(int v[1]) {
    return *v * 3;
}

$fun int teste3(int x) {
    assert(x < 52 && "X most be less than 52");
    return x;
}

int main(void) {
    static uint8_t buffer[2*_Mega_Bytes] = {0};
    buffer[0] += 1;

    int z = teste3(51);
    z *= 2;
    return 0;
}
