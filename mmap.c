#include "ale.h"

static inline void run(void) {
    size_t contents_len = 0;

    char *contents = mmap_open("measurements10k.txt", "r", &contents_len);
    {
        printf("\n%zu\n", contents_len);

        printf("%s\n", &contents[contents_len - 255]);

        printf("len: %zu\n", contents_len);
    }
    mmap_close(contents, contents_len);

    printf("\nDone\n");
}

int main(void)
{
    run();
}
