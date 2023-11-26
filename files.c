#include "ale_io.h"

int main(void) {
    static uint8_t buffer[2*_Mega_Bytes];

    arena_t perm = newarena(sizeof(buffer), buffer);
    
    mstr_t contents = file_to_buffer(&perm, "exemplo.txt");
    printf("%s", contents);

    vector64_t lines = slice_into_nonempty_lines(&perm, contents);
    cstr_t *data = vec_data_as_cstr(&lines);

    qsort(data, (uint64_t) lines.len, 
        sizeof(int64_t), void_compare_strings);

    for (int i = 0; i < lines.len; ++i) {
        printf("%d: %s\n", i, data[i]);
    }

    buffer_to_file(data[0], "first.txt");

    lines_to_file(lines, "sorted.txt");
}
