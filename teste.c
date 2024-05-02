#include "ale.h"

int main(void) {
    file_create("teste.txt", 13ll * 1024ll * 1024ll * 1024ll);
    printf("size: %" PRId64 "\n", filename_size("teste.txt"));
    printf("Done \n");
}
