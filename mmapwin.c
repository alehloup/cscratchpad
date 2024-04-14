#include <stdio.h>
#include <windows.h>
#include <time.h>
#include "ale.h"


_proc run(void) {
    Mmap mmap_info = mmap_open(S("C:/Users/Aleh/1brc_java/measurements1b.txt")); 
    {
        printf("\n%lld\n", mmap_info.contents.len);

        printf("%s\n", &mmap_info.contents.elements[13795387400]);
    }
    mmap_close(mmap_info);

    printf("\nDone\n");
}

int main(void)
{
    START_WATCH;
    run();
    STOP_WATCH;
}
