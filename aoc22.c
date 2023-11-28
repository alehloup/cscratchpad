#include "ale_io.h"

$proc _1(arena_t arena[1]) {
    vector64_t lines = file_to_lines(arena, "aoc.txt");
    cstr_t *data = vec_data_as_cstr(&lines);

    int64_t best3[3] = {0, 0, 0};
    int64_t cur = 0, elf = 0;
    for (int i = 0; i < lines.len; ++i) {
        cstr_t line = data[i];
        if (is_empty_string(line)) {
            if (cur > best3[0]) {
                best3[2] = best3[1];
                best3[1] = best3[0];
                best3[0] = cur;
            } else if (cur > best3[1]) {
                best3[2] = best3[1];
                best3[1] = cur;
            } else if (cur > best3[2]) {
                best3[2] = cur;
            }
            printf("\n %lldelf: %lld \n", ++elf, cur);
            cur = 0;
            continue;
        }
        cur += cstr_to_num(line);
    }
    printf("%lld + %lld + %lld = %lld\n", 
        best3[0], best3[1], best3[2], best3[0] + best3[1] + best3[2]);
}

int main() {
    static uint8_t mem[128*_Mega_Bytes] = {0};
    arena_t arena = newarena(128*_Mega_Bytes, mem);
    _1(&arena);

    return 0;
}
