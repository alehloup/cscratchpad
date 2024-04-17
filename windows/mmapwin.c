#include "../ale.h"


proc_ run(void) {
    struct mmap_file_t mmap_info = mmap_open("C:/Users/Aleh/1brc_java/measurements1b.txt"); 
    {
        printf("\n%lld\n", mmap_info.filesize);

        printf("%s\n", &mmap_info.contents[13795387400]);
    }
    mmap_close(mmap_info);

    printf("\nDone\n");
}

int main(void)
{
    start_benchclock();
    run();
    stop_benchclock();
}
