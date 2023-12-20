#include <stdio.h>
#include "ale.h"

#define bigbuffer_cap 4096

#define array_cap 16

int main(void) {
    static bufferchar bigbuffer[bigbuffer_cap] = {0};
    static len32 bigbuffer_len = 0;
    
    static cstr array[array_cap] = {" "};
    static len32 array_len = 0;

    for (idx32 i = 0; i < 16; ++i) {
        array[array_len++] = &bigbuffer[bigbuffer_len];
        bigbuffer_len += sprintf_s(
            &bigbuffer[bigbuffer_len], bigbuffer_cap, 
            "123 |%d| 321", i
        );
        ++bigbuffer_len; // count the '\0'
    }

    for (idx32 i = 0; i < array_len; ++i) {
        printf("%s\n", array[i]);
    }

    printf("\n");
    for (idx32 i = 0; i < bigbuffer_len; ++i) {
        if (bigbuffer[i] == 0) {
            printf("|_|");
        } else {
            printf("%c", bigbuffer[i]);
        }
    }
    printf("\n");


    return 0;
}
