#include "ale_io.h"

static u8 mem[128*MBs_] = {0};
static Arena arena = {0, 0};

_fun i64 per_line(mstr line) {
    mstr card = split(&arena, line, ':')[1];
    mstr *winnums_cardnums = split(&arena, card, '|');
    mstr winnums = winnums_cardnums[0], cardnums = winnums_cardnums[1];
    mstr *wins = split(&arena, winnums, ' '), 
         *nums = split(&arena, cardnums, ' ');
    mstr *wins_set = NEW_SET(&arena, mstr, 0);
    
    for (int i = 0; i < hd_len_(wins); ++i) {
        printf("{%s} ", wins[i]);
        discard_ hset_set(wins_set, wins[i]);
    }

    printf (" | ");

    int nwins = 0;
    for (int i = 0; i < hd_len_(nums); ++i) {
        if (hset_get(wins_set, nums[i])) {
            printf("|%s| ", nums[i]);
            ++nwins;
        }
    } 
    printf("#%d \n", nwins);

    return nwins;
}

//AoC 4
_proc aoc(ci32 lineslen, mstr lines[1]) {
    static i64 nscratches[256] = {0};
    i64 sum = 0;
    for (int i = 0; i < countof(nscratches); ++i) 
        nscratches[i] = 1;
    
    for (int i = 0; i < lineslen; ++i) {
        i64 wins = per_line(lines[i]);

        for (int j = 0; j < wins; ++j) {
            nscratches[i+j+1] += nscratches[i];
        } 
    }

    for (int i = 0; i < lineslen; ++i) {
        sum += nscratches[i];
        printf("%d:%lld ", i+1, nscratches[i]);
    }

    printf("\n\n Sum: %lld \n", sum);
} 


i32 main(void) {
    arena = new_arena(128*MBs_, mem);

    mstr *lines = file_to_lines(&arena, "./txts/aoc.txt");
    clock_t start = clock();
    aoc(hd_len_(lines), lines);
    print_clock(start);

    return 0;
}
