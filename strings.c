#include <stdio.h>
#include "ale.h"

_proc test_buffers(void) {
    String string = S("Alessandro");
    char bdata[256] = {0};
    Buffer buff = _array_init(bdata);

    buffer_append(&buff, string);
    buffer_append(&buff, S(" Luiz"));
    buffer_append(&buff, S(" Stamatto"));

    buffer_set(&buff, S("Woa"));

    printf("%s |%lld| \n", buff.data, buff.len);
}

_proc test_to_lines(void) {
    String text = S("Alessandro \n Luiz\n Stamatto \n Ferreira");
    String blines[256] = {S("")};
    Array_string lines = _array_init(blines);

    to_lines(&lines, text);

    print_s(trim(blines[0])); printn;
    print_s(trim(blines[1])); printn;
    print_s(trim(blines[2])); printn;
    print_s(trim(blines[3])); printn;
}

int main(void) {
    test_buffers();
    test_to_lines();

    return 0;
}
