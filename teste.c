#include <stdio.h>

static void another(void) {
    enum { tam = 3 };

    int arr[tam] = {0};
    int last = tam-1;

    arr[last] = tam;

    printf("%d\n", arr[last]);
}

int main(void) {
    enum { tam = 5 };

    int arr[tam] = {0};
    int last = tam-1;

    double x = 0.5;
    printf("%f %f\n", x, x + 1);

    arr[last] = tam;

    printf("%d\n", arr[last]);

    another();

    last = last << 1;
    printf("last << 1 : %d\n", last);



    return 0;
}
