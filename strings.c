#include <stdio.h>
#include "ale.h"

_proc test_buffers(void) {
    String string = S("Alessandro");
    _new_array(buff, char, 256);

    buffer_append(&buff, string);
    buffer_append(&buff, S(" Luiz"));
    buffer_append(&buff, S(" Stamatto"));

    buffer_set(&buff, S("Woa"));

    printf("%s |%lld| \n", buff.elements, buff.len);
}

_proc test_to_lines(void) {
    String text = S("Alessandro \n Luiz\n Stamatto \n Ferreira");
    _new_array(lines, String, 256);

    to_lines(&lines, text);

    string_print(trim(lines.elements[0])); printn;
    string_print(trim(lines.elements[1])); printn;
    string_print(trim(lines.elements[2])); printn;
    string_print(trim(lines.elements[3])); printn;
}

_proc test_split(void) {
    String text = S("Alessandro  Luiz Stamatto  Ferreira");
    _new_array(words, String, 256);

    split(&words, text, ' ');

    string_print(trim(words.elements[0])); printn;
    string_print(trim(words.elements[1])); printn;
    string_print(trim(words.elements[2])); printn;
    string_print(trim(words.elements[3])); printn;
}

_proc test_string_eq(void) {
    String s1 = S("Alessandro"), s2 = S("Alessandr");

    printf("string_cmp: %d\n", string_cmp(s1, s2));
    
}

_proc test_memoset(void) {
    int64_t x = 5;
    String s = S("Alessandro");

    printf("Antes: %lld %s|%lld \n", x, s.text, s.len);

    x = ZERO_VAL(int64_t); //memozero(&x, sizeof(x));
    s = ZERO_VAL(typeof(s)); //memozero(&s, sizeof(s));

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

    for (int64_t i = 0; i < lines.len; ++i) {
        printf("%lld: ", i); string_print(lines.elements[i]); printn;
    }

    lines_to_file(S("linhas.txt"), lines);
}

_proc test_filewrite(void) {
    string_to_file(S("saida.txt"), S("Woa\nTestando arquivos!\n"));
}

int32_t main(void) {
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
