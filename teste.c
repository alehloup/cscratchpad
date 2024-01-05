#include <stdio.h>
#include "ale.h"

#define bigbuffer_cap 4096

#define array_cap 16

int main(void) {
    static char bigbuffer[bigbuffer_cap] = {0};
    static Long bigbuffer_len = 0;

    static char smallbuffer[32] = {0};
    
    static cstr array[array_cap] = {" "};
    static int array_len = 0;

    for (int i = 0; i < 16; ++i) {
        sprintf_s(smallbuffer, 32, "123 |%d| 321", i);
        array[array_len++] = 
            save_str_to_buffer(smallbuffer, 
                bigbuffer, &bigbuffer_len, bigbuffer_cap);
    }

    for (int i = 0; i < array_len; ++i) {
        printf("%s\n", array[i]);
    }

    printf("\n");
    for (int i = 0; i < 256; ++i) {
        if (bigbuffer[i] == 0) {
            printf("|_|");
        } else {
            printf("%c", bigbuffer[i]);
        }
    }
    printf("\n");


    return 0;
}
