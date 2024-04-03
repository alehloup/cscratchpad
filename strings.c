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

    printf("%s |%lld| \n", buff.elements, buff.len);
}

_proc test_to_lines(void) {
    String text = S("Alessandro \n Luiz\n Stamatto \n Ferreira");
    String blines[256];
    Strings lines = A(blines);

    to_lines(&lines, text);

    string_print(trim(blines[0])); printn;
    string_print(trim(blines[1])); printn;
    string_print(trim(blines[2])); printn;
    string_print(trim(blines[3])); printn;
}

_proc test_split(void) {
    String text = S("Alessandro  Luiz Stamatto  Ferreira");
    String bwords[256];
    Strings words = A(bwords);

    split(&words, text, ' ');

    string_print(trim(bwords[0])); printn;
    string_print(trim(bwords[1])); printn;
    string_print(trim(bwords[2])); printn;
    string_print(trim(bwords[3])); printn;
}

_proc test_string_eq(void) {
    String s1 = S("Alessandro"), s2 = S("Alessandr");

    printf("memocmp: %d\n", memocmp(&s1, &s2, sizeof(s1)));
    
}

_proc test_memoset(void) {
    Long x = 5;
    String s = S("Alessandro");

    printf("Antes: %lld %s|%lld \n", x, s.text, s.len);

    memozero(&x, sizeof(x));
    memozero(&s, sizeof(s));

    printf("Depois: %lld %s|%lld \n", x, s.text, s.len);
}

_proc test_fileread(void) {
    _new_array(b, Byte, 2048);
    file_to_buffer(S("commands.txt"), &b);

    printf("|%lld|\n%s\n", b.len, b.elements);
}

_proc test_fileread_to_lines(void) {
    _new_array(b, Byte, 2048);
    _new_array(lines, String, 64);

    file_to_lines(S("commands.txt"), &lines, &b);

    for (Long i = 0; i < lines.len; ++i) {
        printf("%lld: ", i); string_print(lines.elements[i]); printn;
    }

    lines_to_file(S("linhas.txt"), lines);
}

_proc test_filewrite(void) {
    string_to_file(S("saida.txt"), S("Woa\nTestando arquivos!\n"));
}

int main(void) {
    test_buffers();
    test_to_lines();
    test_split();
    test_string_eq();
    test_memoset();
    test_fileread();
    test_fileread_to_lines();
    test_filewrite();

    return 0;
}
