#include "ale.h"

proc_ test_buffers(void) {
    struct sslice_t string = to_sslice("Alessandro");
    char buff[256];
    int64_t buff_len = 0;

    buffer_appendslice(ARRCAP_(buff), buff, &buff_len, string);
    buffer_appendslice(ARRCAP_(buff), buff, &buff_len, to_sslice(" Luiz"));
    buffer_appendslice(ARRCAP_(buff), buff, &buff_len, to_sslice(" Stamatto"));

    printf("%s |%" PRId64 "| \n", buff, buff_len);
}

proc_ test_to_lines(void) {
    struct sslice_t text = to_sslice("Alessandro \n Luiz\n Stamatto \n Ferreira");
    
    struct sslice_t lines[256];
    int64_t lines_len = 0;

    to_lines(text, ARRCAP_(lines), lines, &lines_len);

    sslice_print(trimmed(lines[0]));
    sslice_print(trimmed(lines[1]));
    sslice_print(trimmed(lines[2]));
    sslice_print(trimmed(lines[3]));
}

proc_ test_split(void) {
    struct sslice_t text = to_sslice("Alessandro  Luiz Stamatto  Ferreira");

    struct sslice_t words[256];
    int64_t words_len = 0;

    split(text, ' ', ARRCAP_(words), words, &words_len);

    sslice_print(trimmed(words[0]));
    sslice_print(trimmed(words[1]));
    sslice_print(trimmed(words[2]));
    sslice_print(trimmed(words[3]));
}

proc_ print_sslicecmp(struct sslice_t a, struct sslice_t b) {
        printf("sslicecmp, ");
        
        sslice_printend(a, " ");
        printf("cmp ");
        sslice_printend(b, "");
        // ==
        printf(" %d \n", sslice_cmp(a, b));
}

proc_ test_string_eq(void) {
    print_sslicecmp(to_sslice("Alessandro"), to_sslice("Alessandr"));
    print_sslicecmp(to_sslice("Alessandro"), to_sslice("Alessandro"));
    print_sslicecmp(to_sslice("Sarah"), to_sslice("Alexi"));
    print_sslicecmp(to_sslice("Alex"), to_sslice("Sara"));
}

proc_ test_fileread(void) {
    char b[2048];
    int64_t b_len = 0;
    file_to_buffer("commands.txt", ARRCAP_(b), b, &b_len);

    printf("|%" PRId64 "|\n%s\n", b_len, b);
}

proc_ test_fileread_to_lines(void) {
    char b[2048];
    int64_t b_len = 0;

    struct sslice_t lines[256];
    int64_t lines_len = 0;

    file_to_lines("commands.txt", ARRCAP_(b), b, &b_len, ARRCAP_(lines), lines, &lines_len);

    for (int64_t i = 0; i < lines_len; ++i) {
        printf("%" PRId64 ": ", i); sslice_print(lines[i]);
    }

    lines_to_file(ARRCAP_(lines), lines, &lines_len, "linhas.txt");
}

proc_ test_filewrite(void) {
    sslice_to_file(to_sslice("Woa\nTestando arquivos!\n"), "saida.txt");
}

int32_t main(void) {
    test_buffers();
    test_to_lines();
    test_split();
    test_string_eq();
    test_fileread();
    test_fileread_to_lines();
    test_filewrite();

    printf("\nTested string functions.\n");

    return 0;
}
