#include "ale.h"

proc_ test_buffers(void) {
    int printed = printf("\n TEST %s \n", __func__);
    struct sslice_t string = to_sslice("Alessandro");
    char buff[256];
    size_t buff_len = 0;

    buffer_appendslice(arrsizeof(buff), buff, &buff_len, string);
    buffer_appendslice(arrsizeof(buff), buff, &buff_len, to_sslice(" Luiz"));
    buffer_appendslice(arrsizeof(buff), buff, &buff_len, to_sslice(" Stamatto"));

    printf("%s |%zu| \n", buff, buff_len);
    (void) printed;
}

proc_ test_to_lines(void) {
    int printed = printf("\n TEST %s \n", __func__);
    struct sslice_t text = to_sslice("Alessandro \n Luiz\n Stamatto \n Ferreira");
    
    struct sslice_t lines[256];
    size_t lines_len = 0;

    to_lines(text, arrsizeof(lines), lines, &lines_len);

    sslice_print(trimmed(lines[0]));
    sslice_print(trimmed(lines[1]));
    sslice_print(trimmed(lines[2]));
    sslice_print(trimmed(lines[3]));
    (void) printed;
}

proc_ test_split(void) {
    int printed = printf("\n TEST %s \n", __func__);
    struct sslice_t text = to_sslice("Alessandro  Luiz Stamatto  Ferreira");

    struct sslice_t words[256];
    size_t words_len = 0;

    split(text, ' ', arrsizeof(words), words, &words_len);

    sslice_print(trimmed(words[0]));
    sslice_print(trimmed(words[1]));
    sslice_print(trimmed(words[2]));
    sslice_print(trimmed(words[3]));
    (void) printed;
}

proc_ print_sslicecmp(struct sslice_t a, struct sslice_t b) {
    int printed = printf("\n TEST %s \n", __func__);
    printf("sslicecmp, ");
    
    sslice_printend(a, " ");
    printf("cmp ");
    sslice_printend(b, "");
    // ==
    printf(" %d \n", sslice_cmp(a, b));
    (void) printed;
}

proc_ test_string_eq(void) {
    int printed = printf("\n TEST %s \n", __func__);
    print_sslicecmp(to_sslice("Alessandro"), to_sslice("Alessandr"));
    print_sslicecmp(to_sslice("Alessandro"), to_sslice("Alessandro"));
    print_sslicecmp(to_sslice("Sarah"), to_sslice("Alexi"));
    print_sslicecmp(to_sslice("Alex"), to_sslice("Sara"));
    (void) printed;
}

proc_ test_fileread(void) {
    int printed = printf("\n TEST %s \n", __func__);
    char b[2048];
    size_t b_len = 0;
    file_to_buffer("commands.txt", arrsizeof(b), b, &b_len);

    printf("|%zu|\n%s\n", b_len, b);
    (void) printed;
}

proc_ test_fileread_to_lines(void) {
    int printed = printf("\n TEST %s \n", __func__);
    char b[2048];
    size_t b_len = 0;

    struct sslice_t lines[256];
    size_t lines_len = 0;

    file_to_lines("commands.txt", arrsizeof(b), b, &b_len, arrsizeof(lines), lines, &lines_len);

    for (size_t i = 0; i < lines_len; ++i) {
        printf("%zu: ", i); sslice_print(lines[i]);
    }

    lines_to_file(arrsizeof(lines), lines, &lines_len, "linhas.txt");
    (void) printed;
}

proc_ test_filewrite(void) {
    int printed = printf("\n TEST %s \n", __func__);
    sslice_to_file(to_sslice("Woa\nTestando arquivos!\n"), "saida.txt");
    (void) printed;
}

proc_ test_subss(void) {
    int printed = printf("\n TEST %s \n", __func__);
    size_t end = arrsizeof("Alessandro");
    struct sslice_t nome = {end - 1, "Alessandro"};

    sslice_print(nome);
    sslice_print(subss(nome, 2, (int)end));
    
    sslice_print(subss(nome, -2, (int)end));
    (void) printed;
}

proc_ test_literal_sslice(void) {
    sslice_print(to_sslice("Alex"));
    sslice_print(to_sslice("Sara"));
    sslice_print(to_sslice("1234567"));
}

int main(void) {
    test_buffers();
    test_to_lines();
    test_split();
    test_string_eq();
    test_fileread();
    test_fileread_to_lines();
    test_filewrite();
    test_subss();
    test_literal_sslice();

    printf("\nTested string functions.\n");

    return 0;
}
