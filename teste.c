#include <stdio.h>

struct woa {int x;};

static inline int f(struct woa * stc) {
    return stc->x;
}

int main(void) {
    struct woa stc = {0};
    printf("Woa: %d\n", f(&stc));
}
