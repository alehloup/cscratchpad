#include "ale_io.h"

i32 main(void) {
    static u8 buffer[2*MBs_];

    Arena perm = new_arena(isizeof(buffer), buffer);
    
    mstr contents = file_to_buffer(&perm, "./txts/exemplo.txt");
    printf("%s", contents);

    {
        mstr *lines = into_lines(&perm, contents);
        vec_sort_cstr(lines);

        
        for (i32 i = 0; i < hd_(lines)->len; ++i) {
            printf("%d: %s\n", i, lines[i]);
        }
        buffer_to_file(lines[0], "./txts/first.txt");
        

        lines_to_file(lines, "./txts/sorted.txt");
    }
}
