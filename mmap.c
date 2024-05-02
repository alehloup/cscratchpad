#include "ale.h"

proc_ run(void) {

    #ifdef _WINDOWS_
        #define HOME "C:/Users/Aleh"
    #else
        #define HOME "/mnt/c/Users/Aleh"
    #endif

    struct mmap_t mmap_info = mmap_open(HOME "/1brc_java/measurements1b.txt"); 
    {
        printf("\n%" PRId64 "\n", mmap_info.filesize);

        printf("%s\n", &mmap_info.contents[13795387400]);
    }
    mmap_close(mmap_info);

    printf("\nDone\n");
}

int main(void)
{
    run();
}
