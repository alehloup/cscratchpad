#include "ale.h"

proc_ test_buffers(void) {
    struct sslice_t string = sslice_("Alessandro");
    arrnew_(buff, char, 256);

    buffer_append(arrarg_(buff), string);
    buffer_append(arrarg_(buff), sslice_(" Luiz"));
    buffer_append(arrarg_(buff), sslice_(" Stamatto"));

    buffer_set(arrarg_(buff), sslice_("Woa"));

    printf("%s |%lld| \n", buff, *buff_len);
}

proc_ test_to_lines(void) {
    struct sslice_t text = sslice_("Alessandro \n Luiz\n Stamatto \n Ferreira");
    arrnew_(lines, struct sslice_t, 256);

    to_lines(text, arrarg_(lines));

    sslice_print(trimmed(lines[0])); printn;
    sslice_print(trimmed(lines[1])); printn;
    sslice_print(trimmed(lines[2])); printn;
    sslice_print(trimmed(lines[3])); printn;
}

proc_ test_split(void) {
    struct sslice_t text = sslice_("Alessandro  Luiz Stamatto  Ferreira");
    arrnew_(words, struct sslice_t, 256);

    split(text, ' ', arrarg_(words));

    sslice_print(trimmed(words[0])); printn;
    sslice_print(trimmed(words[1])); printn;
    sslice_print(trimmed(words[2])); printn;
    sslice_print(trimmed(words[3])); printn;
}

proc_ print_sslice_cmp(struct sslice_t a, struct sslice_t b) {
        printf("sslice_cmp, ");
        
        sslice_print(a);
        printf(" cmp ");
        sslice_print(b);
        // ==
        printf(" %d \n", sslice_cmp(a, b));
}

proc_ test_string_eq(void) {
    print_sslice_cmp(sslice_("Alessandro"), sslice_("Alessandr"));
    print_sslice_cmp(sslice_("Alessandro"), sslice_("Alessandro"));
    print_sslice_cmp(sslice_("Sarah"), sslice_("Alexi"));
    print_sslice_cmp(sslice_("Alex"), sslice_("Sara"));
}

proc_ test_fileread(void) {
    arrnew_(b, char, 2048);
    file_to_buffer("commands.txt", arrarg_(b));

    printf("|%lld|\n%s\n", *b_len, b);
}

proc_ test_fileread_to_lines(void) {
    arrnew_(b, char, 2048);
    arrnew_(lines, struct sslice_t, 64);

    file_to_lines("commands.txt", arrarg_(b), arrarg_(lines));

    for (int64_t i = 0; i < (*lines_len); ++i) {
        printf("%lld: ", i); sslice_print(lines[i]); printn;
    }

    lines_to_file(arrarg_(lines), "linhas.txt");
}

proc_ test_filewrite(void) {
    sslice_to_file(sslice_("Woa\nTestando arquivos!\n"), "saida.txt");
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
