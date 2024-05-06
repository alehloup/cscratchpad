#include "ale.h"

proc_ run(void) {

    struct mmap_t mmap_info = mmap_open("measurements1b.txt");
    {
        printf("\n%zu\n", mmap_info.filesize);

        printf("%s\n", &mmap_info.contents[mmap_info.filesize - 255]);
    }
    mmap_close(mmap_info);

    printf("len: %zu\n", filename_size("measurements1b.txt"));

    printf("\nDone\n");
}

int main(void)
{
    run();
}
