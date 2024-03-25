#include <stdio.h>
#include "ale.h"

_proc test_buffers(void) {
    String string = S("Alessandro");
    char bdata[256] = {0};
    Buffer buff = A(bdata);

    buffer_append(&buff, string);
    buffer_append(&buff, S(" Luiz"));
    buffer_append(&buff, S(" Stamatto"));

    buffer_set(&buff, S("Woa"));

    printf("%s |%lld| \n", buff.data, buff.len);
}

_proc test_to_lines(void) {
    String text = S("Alessandro \n Luiz\n Stamatto \n Ferreira");
    String blines[256] = {S("")};
    Strings lines = A(blines);

    to_lines(&lines, text);

    string_print(trim(blines[0])); printn;
    string_print(trim(blines[1])); printn;
    string_print(trim(blines[2])); printn;
    string_print(trim(blines[3])); printn;
}

_proc test_split(void) {
    String text = S("Alessandro  Luiz Stamatto  Ferreira");
    String bwords[256] = {S("")};
    Strings words = A(bwords);

    split(&words, text, ' ');

    string_print(trim(bwords[0])); printn;
    string_print(trim(bwords[1])); printn;
    string_print(trim(bwords[2])); printn;
    string_print(trim(bwords[3])); printn;
}

int main(void) {
    test_buffers();
    test_to_lines();
    test_split();

    return 0;
}
