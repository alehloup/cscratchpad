#include "aleh/aleh.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void print_str(arena a) {
    str *pstring = new(&a, 1, str);

    pstring->data = (char *) "Alessandro";
    pstring->len = lengthof(pstring->data);

    printf("%s %zd", pstring->data, pstring->len);
}

int main(void) {
    str string = S("Alessandro Stamatto");

    printf("%s %zd \n", string.data, string.len);

    printf("%d \n", str_equal(S("Alessandro Stamatt"), string));

    char bytes[52] = {0};
    arena a = {bytes, bytes + 52};

    print_str(a);

    return 0;
}

