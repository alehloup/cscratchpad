#include <stdio.h>
#include "ale.h"

#define bigbuffer_cap 4096

#define array_cap 16
#define strtmp_cap 16

int main(void) {
    static bufferchar bigbuffer[bigbuffer_cap] = {0};
    static len32 bigbuffer_len = 0;
    
    static cstr array[array_cap] = {" "};
    static len32 array_len = 0;

    static bufferchar strtmp[strtmp_cap] = {0};

    for (idx32 i = 0; i < 16; ++i) {
        len32 len = sprintf_s(strtmp, 16, "123 |%d| 321", i);
        
        array[array_len++] = &bigbuffer[bigbuffer_len];
        bigbuffer_len += cstrcpy(&bigbuffer[bigbuffer_len], strtmp, len);
    }

    for(idx32 i = 0; i < array_len; ++i) {
        printf("%s\n", array[i]);
    }


    return 0;
}
