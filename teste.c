#include "aleh/aleh.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void test_strings(arena a) {
    str string = S("Alessandro Stamatto");
    printf("%s %zd \n", string.data, string.len);
    printf("%d \n", equal(S("Alessandro Stamatt"), string));

    str *pstring = new(&a, 1, str);

    pstring->data = (char *) "Alessandro";
    pstring->len = cstrlen(pstring->data);

    printf("%s %zd\n", pstring->data, pstring->len);

    str string2 = copy(&a, string);
    printf("%d %s %s \n", equal(string, string2), string.data, string2.data);
    string2.data[3] = 'h';
    printf("%d %s %s \n", equal(string, string2), string.data, string2.data);
}

#define print_equals(format, a, b) \
    printf(format " == " format " (%d)\n", a, b, equal(a, b)); \
    printf(format " == " format " (%d)\n", a, b+1, equal(a, b+1))

static void test_equals() {
    char ca = 'a', cb = 'a';
    int ia = 5, ib = 5;
    ssize_t sa = -52, sb = -52;
    size_t ua = 52, ub = 52;
    float fa = 5.2f, fb = 5.2f;
    double da = 52.52, db = 52.52;

    print_equals("%c", ca, cb);
    print_equals("%d", ia, ib);
    print_equals("%zd", sa, sb);
    print_equals("%zu", ua, ub);
    print_equals("%f", (double) fa, (double) fb);
    print_equals("%f", da, db);
}

#define ROOTWIN "D:/"
#define ROOTLIN "/mnt/d/"

int main(void) {
    char *data = malloc(16*GB);
    arena a = {data, data + 16*GB};
    
    test_strings(a);
    test_equals();
    printf("\n");

    str contents = filestr(&a, S(ROOTWIN "1brc_java/measurements10k.txt"));
    println(contents.len);

    contents = filestr(&a, S(ROOTLIN "1brc_java/measurements10k.txt"));
    println(contents.len);

    return 0;
}

