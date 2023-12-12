#include <math.h>
#include "ale_io.h"

#define print(...)  //printf(__VA_ARGS__)
#define printn(...) //print(__VA_ARGS__); printf("\n")

static u8 mem[128*MBs_] = {0};
static Arena A = {0, 0};

_fun i32 per_line(mstr line) {
    mstr *numbers_str = split(&A, line, ' ');
    i32 sequence_size = hd_len_(numbers_str), seqsum = 0;
    NEW_VEC(&A, sequence, i32);
    NEW_VEC(&A, sums, i32);

    for (int i = 0; i < hd_len_(numbers_str); ++i) {
        sscanf_s(numbers_str[i], "%d ", vec_inc_ref(sequence));
    }

    print_vec(sequence, " %d ");
    printf("------------------------\n");
    
    while(sequence_size > 0 && sequence[sequence_size-1]) {
        seqsum += sequence[--sequence_size];
        vec_append(sums, sequence[sequence_size]);
        for (int i = 0; i < sequence_size; ++i) {
            sequence[i] = sequence[i+1] - sequence[i];
        }
        --hd_(sequence)->len;
        print_vec(sequence, " %d ");
        printf("------------------------\n");
    }
    printf("Sums: "); print_vec(sums, " %d ");
   
    return seqsum;
}

//AoC 9
_proc aoc(ci32 lineslen, mstr lines[1]) {
    i64 sum = 0;
    for (int iline = 0; iline < lineslen; ++iline) {
        mstr line = lines[iline];
        i32 seqnum = 0;
        if (is_empty_string(line)) {
            continue;
        }
        seqnum = per_line(line);
        printf(" [%d] \n\n\n", seqnum);
        sum += seqnum;
    }
    printf("Sum: %lld \n", sum);
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
