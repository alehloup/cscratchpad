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
    static i32 sequence[64] = {0}, history[64] = {0};
    i32 sequence_len = 0, history_len = 0, seqsum = 0;

    for (int i = 0; i < hd_len_(numbers_str); ++i) {
        sscanf_s(numbers_str[i], "%d ", &sequence[sequence_len++]);
    }
    
    history[history_len++] = sequence[0];

    while(sequence_len > 1) {
        i32 cur = turn_into_subseq(sequence_len--, sequence);
        history[history_len++] = cur;
    }
    
    seqsum = 0;
    for (int i = history_len - 1; i >= 0; --i) {
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
