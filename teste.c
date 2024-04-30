#include "ale.h"

int main(void) {
    float x = 81; (void)x;
    int32_t y[] = {1, 2, 3};

    //printf("%f\n", sqrtapproximate_newton(x));

    printf("Array Cap: %lld\n", CAP_(y));
    sleep_(5);
    printf("Terminei de dormir\n");
}
