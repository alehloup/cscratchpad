#include <stddef.h>
#include <stdio.h>

inline static size_t minusone(int x) {
    if (x % 2 == 0) {
        return (size_t)-1;
    }

    return 0;
}

int main(void) {
    int x = 0;
    int printed = printf("\n > ");
    int scanned = scanf("%d", &x);

    size_t pos = minusone(x);

    if (pos == -1ull) {
        printf("Par dá menos um!");
    } else {
        printf("ok");
    }

    (void) printed;
    (void) scanned;
}
