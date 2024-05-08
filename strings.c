#include "ale.h"

proc_ test_buffers(void) {
    int printed = printf("\n TEST %s \n", __func__);
    struct sslice_t string = SS_("Alessandro");
    char buff[256];
    size_t buff_len = 0;

    buffer_appendslice(CAP_(buff), buff, &buff_len, string);
    buffer_appendslice(CAP_(buff), buff, &buff_len, SS_(" Luiz"));
    buffer_appendslice(CAP_(buff), buff, &buff_len, SS_(" Stamatto"));

    printf("%s |%zu| \n", buff, buff_len);
    (void) printed;
}

proc_ test_to_lines(void) {
    int printed = printf("\n TEST %s \n", __func__);
    struct sslice_t text = SS_("Alessandro \n Luiz\n Stamatto \n Ferreira");
    
    struct sslice_t lines[256];
    size_t lines_len = 0;

    to_lines(text, CAP_(lines), lines, &lines_len);

    sslice_print(trimmed(lines[0]));
    sslice_print(trimmed(lines[1]));
    sslice_print(trimmed(lines[2]));
    sslice_print(trimmed(lines[3]));
    (void) printed;
}

proc_ test_split(void) {
    int printed = printf("\n TEST %s \n", __func__);
    struct sslice_t text = SS_("Alessandro  Luiz Stamatto  Ferreira");

    struct sslice_t words[256];
    size_t words_len = 0;

    split(text, ' ', CAP_(words), words, &words_len);

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
    print_sslicecmp(SS_("Alessandro"), SS_("Alessandr"));
    print_sslicecmp(SS_("Alessandro"), SS_("Alessandro"));
    print_sslicecmp(SS_("Sarah"), SS_("Alexi"));
    print_sslicecmp(SS_("Alex"), SS_("Sara"));
    (void) printed;
}

proc_ test_fileread(void) {
    int printed = printf("\n TEST %s \n", __func__);
    char b[2048];
    size_t b_len = 0;
    file_to_buffer("commands.txt", CAP_(b), b, &b_len);

    printf("|%zu|\n%s\n", b_len, b);
    (void) printed;
}

proc_ test_fileread_to_lines(void) {
    int printed = printf("\n TEST %s \n", __func__);
    char b[2048];
    size_t b_len = 0;

    struct sslice_t lines[256];
    size_t lines_len = 0;

    file_to_lines("commands.txt", CAP_(b), b, &b_len, CAP_(lines), lines, &lines_len);

    for (size_t i = 0; i < lines_len; ++i) {
        printf("%zu: ", i); sslice_print(lines[i]);
    }

    lines_to_file(CAP_(lines), lines, &lines_len, "linhas.txt");
    (void) printed;
}

proc_ test_filewrite(void) {
    int printed = printf("\n TEST %s \n", __func__);
    sslice_to_file(SS_("Woa\nTestando arquivos!\n"), "saida.txt");
    (void) printed;
}

proc_ test_subss(void) {
    int printed = printf("\n TEST %s \n", __func__);
    size_t end = CAP_("Alessandro");
    struct sslice_t nome = {end - 1, "Alessandro"};

    sslice_print(nome);
    sslice_print(subss(nome, 2, (int)end));
    
    sslice_print(subss(nome, -2, (int)end));
    (void) printed;
}

proc_ test_literal_sslice(void) {
    sslice_print(SS_("Alex"));
    sslice_print(SS_("Sara"));
    sslice_print(SS_("1234567"));
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
