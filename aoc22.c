#include "ale_io.h"

$proc_rbuffer(1, 2) 
_2(int32_t lineslen, cstr_t lines[]) {

    for (int i = 0; i < lineslen; ++i) {
        printf("%s\n", lines[i]);
    }
}

$proc_rbuffer(1, 2)
_1(int32_t lineslen, cstr_t lines[]) {
    int64_t best3[3] = {0, 0, 0};
    int64_t cur = 0, elf = 0;
    for (int i = 0; i < lineslen; ++i) {
        cstr_t line = lines[i];
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

    vector64_t lines = file_to_lines(&arena, "aoc.txt");
    cstr_t *data = vec_data_as_cstr(&lines);

    _1(lines.len, data);

    return 0;
}
