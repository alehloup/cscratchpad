#include "aleh/mmap.h"

static inline void run(void) {
    size_t contents_len = 0;

    char *contents = mmap_open("measurements10k.txt", "r", &contents_len);
    {
        printf("\n%u KB\n", (unsigned int)contents_len/1024 + 1);

        printf("%s\n", &contents[contents_len - 255]);

        printf("len: %u\n", (unsigned int)contents_len/1024 + 1);
    }
    mmap_close(contents, contents_len);

    printf("\nDone\n");
}

static inline void run_macroed(void) {

    withmmap(contents, "measurements10k.txt", "r") {
        printf("\n%u KB\n", (unsigned int)(size_t)contents_len/1024 + 1);

        printf("%s\n", &contents[(size_t)contents_len - 255]);

        printf("len: %u\n", (unsigned int)(size_t)contents_len/1024 + 1);
    }

    printf("\nDone\n");
}

int main(void)
{
    run();

    run_macroed();

    return 0;
}
