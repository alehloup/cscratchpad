#include "ale_io.h"

i32 main(void) {
    static char buffer[256] = {0};
    static mstr lines[64] = {0};
    i64 buffer_cap = 256;
    i32 lines_cap = 64;
    
    i64 buffer_len = 
        file_to_cstring("./txts/exemplo.txt", 
        buffer_cap, buffer);
    discard_ buffer_len;
    
    printf("%s", buffer);

    {
        i32 lines_len = into_lines(buffer, 
            lines_cap, lines);
        sort_cstrings(lines_len, lines);

        
        for (i32 i = 0; i < lines_len; ++i) {
            printf("%d: %s\n", i, lines[i]);
        }
        cstring_to_file(lines[0], "./txts/first.txt");
        

        lines_to_file(lines_len, lines, "./txts/sorted.txt");
    }
}
