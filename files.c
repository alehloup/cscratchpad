#include "ale_io.h"

i32 main(void) {
    static u8 buffer[2*MBs_];

    arena_t perm = newarena(sizeof(buffer), buffer);
    
    mstr contents = file_to_buffer(&perm, "exemplo.txt");
    printf("%s", contents);

    vector64_t lines = slice_into_nonempty_lines(&perm, contents);
    vec_sort_cstr(lines);

    cstr *data = vec_data_as_cstr(&lines);
    for (i32 i = 0; i < lines.len; ++i) {
        printf("%d: %s\n", i, data[i]);
    }

    buffer_to_file(data[0], "first.txt");

    lines_to_file(lines, "sorted.txt");
}
