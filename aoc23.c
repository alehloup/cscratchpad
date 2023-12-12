#include <math.h>
#include "ale_io.h"

#define print(...)  //printf(__VA_ARGS__)
#define printn(...) //print(__VA_ARGS__); printf("\n")

static u8 mem[128*MBs_] = {0};
static Arena A = {0, 0};

_fun_hot i32 turn_into_subseq(i32 sequence_len, i32 sequence[]) {
    i32 len_minus_one = sequence_len - 1;

    for (int i = 0; i < len_minus_one; ++i) {
        sequence[i] = sequence[i+1] - sequence[i];
    }
    sequence[len_minus_one] = 0;
    return sequence[0];
}

_fun i32 per_line(mstr line) {
    mstr *numbers_str = split(&A, line, ' ');
    i32 sequence_size = hd_len_(numbers_str), seqsum = 0;
    NEW_VEC(&A, sequence, i32);
    NEW_VEC(&A, history, i32);

    for (int i = 0; i < hd_len_(numbers_str); ++i) {
        sscanf_s(numbers_str[i], "%d ", vec_inc_ref(sequence));
    }
    
    vec_append(history, sequence[0]);

    while(sequence_size > 1) {
        i32 cur = turn_into_subseq(sequence_size--, sequence);
        --hd_(sequence)->len;
        vec_append(history, cur);
    }
    
    seqsum = 0;
    for (int i = hd_len_(history) - 1; i >= 0; --i) {
        seqsum = history[i] - seqsum;
    }
   
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
        //printf("%s ", line);
        seqnum = per_line(line);
        //printf(" [%d]\n", seqnum);
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
