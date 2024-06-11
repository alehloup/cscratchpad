#include "./aleh/string.h"

enum { buffer_cap = 256 };

static inline void test_buffers(void) {
    int printed = printf("\n TEST %s \n", __func__);
    struct lenstr_t string = to_lenstr("Alessandro");
    char buff[buffer_cap];
    size_t buff_len = 0;

    buffer_append_lenstr(buffer_cap, buff, &buff_len, string);
    buffer_append_lenstr(buffer_cap, buff, &buff_len, to_lenstr(" Luiz"));
    buffer_append_lenstr(buffer_cap, buff, &buff_len, to_lenstr(" Stamatto"));

    printf("%s |%u| \n", buff, (unsigned int)buff_len);
    (void) printed;
}

static inline void test_to_lines(void) {
    int printed = printf("\n TEST %s \n", __func__);
    struct lenstr_t text = to_lenstr("Alessandro \n Luiz\n Stamatto \n Ferreira");
    
    struct lenstr_t lines[buffer_cap];
    size_t lines_len = 0;

    to_lines(text, buffer_cap, lines, &lines_len);

    lenstr_print(lines[0]);
    lenstr_print(lines[1]);
    lenstr_print(lines[2]);
    lenstr_print(lines[3]);
    (void) printed;
}

static inline void test_split(void) {
    int printed = printf("\n TEST %s \n", __func__);
    struct lenstr_t text = to_lenstr("Alessandro  Luiz Stamatto  Ferreira");

    struct lenstr_t words[buffer_cap];
    size_t words_len = 0;

    split(text, ' ', buffer_cap, words, &words_len);

    lenstr_print(words[0]);
    lenstr_print(words[1]);
    lenstr_print(words[2]);
    lenstr_print(words[3]);
    (void) printed;
}

static inline void print_lenstrcmp(struct lenstr_t a, struct lenstr_t b) {
    int printed = printf("\n TEST %s \n", __func__);
    printf("lenstrcmp, ");
    
    lenstr_printend(a, " ");
    printf("cmp ");
    lenstr_printend(b, "");
    /* == */
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

#define b_cap 2048

static inline void test_subss(void) {
    int printed = printf("\n TEST %s \n", __func__);
    size_t end = strlen("Alessandro");
    struct lenstr_t nome = {0, 0};
    nome.len = end - 1;
    nome.str = "Alessandro";

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
    test_subss();
    test_literal_lenstr();

    printf("\nTested string functions.\n");

    return 0;
}
