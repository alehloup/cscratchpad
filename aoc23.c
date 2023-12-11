#include <math.h>
#include "ale_io.h"

#define print(...)  //printf(__VA_ARGS__)
#define printn(...) //print(__VA_ARGS__); printf("\n")

static u8 mem[128*MBs_] = {0};
static Arena A = {0, 0};

_fun i32 per_line(mstr line) {
    mstr *numbers_str = split(&A, line, ' ');
    ci32 sequence_size = hd_len_(numbers_str);
    NEW_VEC(&A, sequence, i32);

    printf("Sequence size: %d \n", sequence_size);

    for (int i = 0; i < hd_len_(numbers_str); ++i) {
        sscanf_s(numbers_str[i], "%d ", vec_inc_ref(sequence));
    }
    
    printf("Sequence_len: %d \n", hd_len_(sequence));

    vec_append(sequence, 0);
    for (int i = 0; i < hd_len_(sequence); ++i) {
        printf("%d ", sequence[i]);
    }
    printf("\n");

    return 52;
}

//AoC 9
_proc aoc(ci32 lineslen, mstr lines[1]) {
    for (int iline = 0; iline < lineslen; ++iline) {
        mstr line = lines[iline];
        if (is_empty_string(line)) {
            continue;
        }
        printf("%s: %d ", line, per_line(lines[iline]));
    }
} 


i32 main(void) {
    A = new_arena(128*MBs_, mem);

    {
        mstr *lines = file_to_lines(&A, "./txts/aoc.txt");
        clock_t start = clock();
        aoc(hd_len_(lines), lines);
        print_clock(start);
    }

    return 0;
}
