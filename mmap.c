#include "ale.h"

proc_ run(void) {
    size_t contents_len = 0;

    FILE *f = fopen_("measurements10k.txt", "r");
    char *contents = mmap_open(f, "r", &contents_len);
    {
        printf("\n%zu\n", contents_len);

        printf("%s\n", &contents[contents_len - 255]);

        printf("len: %zu\n", filelen_(f));
    }
    mmap_close(contents, contents_len);
    fclose(f);

    printf("\nDone\n");
}

int main(void)
{
    run();
}
