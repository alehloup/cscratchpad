#include "ale.h"

static inline void test_buffers(void) {
    int printed = printf("\n TEST %s \n", __func__);
    struct lenstr_t string = to_lenstr("Alessandro");
    char buff[256];
    size_t buff_len = 0;

    buffer_append_lenstr(arrsizeof(buff), buff, &buff_len, string);
    buffer_append_lenstr(arrsizeof(buff), buff, &buff_len, to_lenstr(" Luiz"));
    buffer_append_lenstr(arrsizeof(buff), buff, &buff_len, to_lenstr(" Stamatto"));

    printf("%s |%zu| \n", buff, buff_len);
    (void) printed;
}

static inline void test_to_lines(void) {
    int printed = printf("\n TEST %s \n", __func__);
    struct lenstr_t text = to_lenstr("Alessandro \n Luiz\n Stamatto \n Ferreira");
    
    struct lenstr_t lines[256];
    size_t lines_len = 0;

    to_lines(text, arrsizeof(lines), lines, &lines_len);

    lenstr_print(trimmed(lines[0]));
    lenstr_print(trimmed(lines[1]));
    lenstr_print(trimmed(lines[2]));
    lenstr_print(trimmed(lines[3]));
    (void) printed;
}

static inline void test_split(void) {
    int printed = printf("\n TEST %s \n", __func__);
    struct lenstr_t text = to_lenstr("Alessandro  Luiz Stamatto  Ferreira");

    struct lenstr_t words[256];
    size_t words_len = 0;

    split(text, ' ', arrsizeof(words), words, &words_len);

    lenstr_print(trimmed(words[0]));
    lenstr_print(trimmed(words[1]));
    lenstr_print(trimmed(words[2]));
    lenstr_print(trimmed(words[3]));
    (void) printed;
}

static inline void print_lenstrcmp(struct lenstr_t a, struct lenstr_t b) {
    int printed = printf("\n TEST %s \n", __func__);
    printf("lenstrcmp, ");
    
    lenstr_printend(a, " ");
    printf("cmp ");
    lenstr_printend(b, "");
    // ==
    printf(" %d \n", lenstr_cmp(a, b));
    (void) printed;
}

static inline void test_string_eq(void) {
    int printed = printf("\n TEST %s \n", __func__);
    print_lenstrcmp(to_lenstr("Alessandro"), to_lenstr("Alessandr"));
    print_lenstrcmp(to_lenstr("Alessandro"), to_lenstr("Alessandro"));
    print_lenstrcmp(to_lenstr("Sarah"), to_lenstr("Alexi"));
    print_lenstrcmp(to_lenstr("Alex"), to_lenstr("Sara"));
    (void) printed;
}

static inline void test_fileread(void) {
    int printed = printf("\n TEST %s \n", __func__);
    char b[2048];
    size_t b_len = 0;
    file_to_buffer("commands.txt", arrsizeof(b), b, &b_len);

    printf("|%zu|\n%s\n", b_len, b);
    (void) printed;
}

static inline void test_fileread_to_lines(void) {
    int printed = printf("\n TEST %s \n", __func__);
    char b[2048];
    size_t b_len = 0;

    struct lenstr_t lines[256];
    size_t lines_len = 0;

    file_to_lines("commands.txt", arrsizeof(b), b, &b_len, arrsizeof(lines), lines, &lines_len);

    for (size_t i = 0; i < lines_len; ++i) {
        printf("%zu: ", i); lenstr_print(lines[i]);
    }

    lines_to_file(arrsizeof(lines), lines, &lines_len, "linhas.txt");
    (void) printed;
}

static inline void test_filewrite(void) {
    int printed = printf("\n TEST %s \n", __func__);
    lenstr_to_file(to_lenstr("Woa\nTestando arquivos!\n"), "saida.txt");
    (void) printed;
}

static inline void test_subss(void) {
    int printed = printf("\n TEST %s \n", __func__);
    size_t end = arrsizeof("Alessandro");
    struct lenstr_t nome = {end - 1, "Alessandro"};

    lenstr_print(nome);
    lenstr_print(subss(nome, 2, (int)end));
    
    lenstr_print(subss(nome, -2, (int)end));
    (void) printed;
}

static inline void test_literal_lenstr(void) {
    lenstr_print(to_lenstr("Alex"));
    lenstr_print(to_lenstr("Sara"));
    lenstr_print(to_lenstr("1234567"));
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
    test_literal_lenstr();

    printf("\nTested string functions.\n");

    return 0;
}
