#include <stdio.h>
#include <stdlib.h>
#include "ale.h"

int main(void) {
    static char buffer[256] = {0};
    static Mstr lines[64] = {0};
    Long buffer_cap = 256;
    int lines_cap = 64;
    
    Long buffer_len = 
        file_to_cstring("./txts/exemplo.txt", 
        buffer_cap, buffer);
    (void) buffer_len;
    
    printf("%s", buffer);

    {
        int lines_len = into_lines(buffer, 
            lines_cap, lines);
        sort_cstrings(lines_len, lines);

        
        for (int i = 0; i < lines_len; ++i) {
            printf("%d: %s\n", i, lines[i]);
        }
        Cstring_to_file(lines[0], "./txts/first.txt");
        

        lines_to_file(lines_len, lines, "./txts/sorted.txt");
    }
}
