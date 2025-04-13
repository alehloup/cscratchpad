#include "aleh/aleh.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void print_str(arena a) {
    str *pstring = new(&a, 1, str);

    pstring->data = (char *) "Alessandro";
    pstring->len = cstrlen(pstring->data);

    printf("%s %zd\n", pstring->data, pstring->len);
}

int main(void) {
    str string = S("Alessandro Stamatto");
    printf("%s %zd \n", string.data, string.len);
    printf("%d \n", str_equal(S("Alessandro Stamatt"), string));

    char bytes[52] = {0};
    arena a = {bytes, bytes + 52};
    print_str(a);

    str string2 = str_copy(&a, string);
    printf("%d %s %s \n", str_equal(string, string2), string.data, string2.data);
    string2.data[3] = 'h';
    printf("%d %s %s \n", str_equal(string, string2), string.data, string2.data);

    return 0;
}

