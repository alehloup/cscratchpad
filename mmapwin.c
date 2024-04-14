#include <stdio.h>
#include <Windows.h>
#include "ale.h"



int main(void)
{
    Mmap mmap_info = mmap_open(S("Commands.txt")); 
    {
        printf("\n%lld\n", mmap_info.contents.len);
    }
    mmap_close(mmap_info);

    printf("\nDone\n");
}
