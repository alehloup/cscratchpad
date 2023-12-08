#include "ale_io.h"

static u8 mem[128*MBs_] = {0};
static Arena A = {0, 0};

_proc apply_conversion(i32 lenseeds, i64 seeds[20], ccstr line) {
    i64num_i32len dst = cstr_to_num(line);
    i64num_i32len src = cstr_to_num(&line[dst.len+1]);
    i64num_i32len len = cstr_to_num(&line[dst.len+1+src.len+1]);

    ci64 addit = dst.num - src.num,
         start = src.num, end = src.num + len.num;

    printf("(%lld|+%lld {%lld->%lld}) ", dst.num, addit, start, end);

    for (int iseed = 0; iseed < lenseeds; ++iseed) {
        i64 seed = seeds[iseed];
        if (seed >= start && seed < end) {
            seeds[iseed] += addit;
            printf("|%lld->%lld| ", seed, seeds[iseed]);
        }
    }
    printf("\n");
}

//AoC 5
_proc aoc(ci32 lineslen, mstr lines[1]) {
    mstr seeds = split(&A, lines[0], ':')[1];
    mstr *seeds_str = split(&A, seeds, ' ');
    i64 *seeds_num = NEW_VEC(&A, i64);

    for (int i = 0; i < hd_len_(seeds_str); ++i) {
        vec_append(seeds_num, cstr_to_num(seeds_str[i]).num);
    }
    assert(hd_len_(seeds_str) == hd_len_(seeds_num));

    printf(" SEEDS: ");
    print_vec(seeds_str, "%s ");

    for (int iline = 2; iline < lineslen; ++iline) {
        ccstr line = lines[iline];
        if (!is_empty_string(line)) {
            if (is_digit(line[0])) {
                apply_conversion(hd_len_(seeds_num), seeds_num, line);
            } else {
                printf("%s\n", line);
            }
        }
    }

    i64 mini = seeds_num[0];
    for (int i = 1; i < hd_len_(seeds_num); ++i) {
        mini = seeds_num[i] < mini ? seeds_num[i] : mini;
    }
    printf("Minimum: %lld\n", mini);
} 


i32 main(void) {
    A = new_arena(128*MBs_, mem);

    mstr *lines = file_to_lines(&A, "./txts/aoc.txt");
    clock_t start = clock();
    aoc(hd_len_(lines), lines);
    print_clock(start);

    return 0;
}
