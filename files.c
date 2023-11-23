#include "ale.h"

static thread_local uint8_t buffer[2*_Mega_Bytes];

int main(void) {
    arena_t perm = newarena(sizeof(buffer), buffer);
    
    modstring contents = file_to_buffer(&perm, "exemplo.txt");
    printf("%s", contents);

    vector64_t lines = slice_into_nonempty_lines(&perm, contents);
    cstring *data = vector_data_as_cstring(&lines);

    qsort(data, (uint64_t) lines.len, 
        sizeof(cstring), void_compare_cstrings);

    for (int i = 0; i < lines.len; ++i) {
        printf("%d: %s\n", i, data[i]);
    }
}
