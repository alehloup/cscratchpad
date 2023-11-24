#include "ale.h"

int main(void) {
    static uint8_t buffer[2*_Mega_Bytes];

    arena_t perm = newarena(sizeof(buffer), buffer);
    
    mstr_t contents = file_to_buffer(&perm, "exemplo.txt");
    printf("%s", contents);

    vector64_t lines = slice_into_nonempty_lines(&perm, contents);

    qsort(lines.data, (uint64_t) lines.len, 
        sizeof(int64_t), void_compare_strings);

    printf("\npos sort\n");

    for (int i = 0; i < lines.len; ++i) {
        printf("%d: %s\n", i, (cstr_t) lines.data[i]);
    }
}
