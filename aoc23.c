#include "ale_io.h"

static u8 mem[128*MBs_] = {0};
static Arena A = {0, 0};

_proc apply_conversion(ccstr line, i32 lenseeds, i64 seeds[20], u8 changed[20]) {
    i64 dst, src, len;
    sscanf_s(line, "%lld %lld %lld", &dst, &src, &len);

    ci64 addit = dst - src,
         start = src, end = src + len;

    printf("(%lld|+%lld {%lld->%lld}) ", dst, addit, start, end);

    for (int iseed = 0; iseed < lenseeds; ++iseed) {
        i64 seed = seeds[iseed];
        if (seed >= start && seed < end && !changed[iseed]) {
            seeds[iseed] += addit;
            changed[iseed] = True;
            printf("#%d|%lld->%lld| ", iseed, seed, seeds[iseed]);
        }
    }
    printf("\n");
}

//AoC 5
_proc aoc(ci32 lineslen, mstr lines[1]) {
    mstr seeds = split(&A, lines[0], ':')[1];
    mstr *seeds_str = split(&A, seeds, ' ');
    i64 *seeds_num = NEW_VEC(&A, i64);
    u8 *changed = NEW_VEC(&A, u8);

    for (int i = 0; i < hd_len_(seeds_str); ++i) {
        vec_append(seeds_num, cstr_to_num(seeds_str[i]).num);
        vec_append(changed, False);
    }
    assert(hd_len_(seeds_str) == hd_len_(seeds_num));

    printf(" SEEDS: ");
    print_vec(seeds_str, "%s ");

    for (int iline = 2; iline < lineslen; ++iline) {
        ccstr line = lines[iline];
        if (!is_empty_string(line)) {
            if (is_digit(line[0])) {
                apply_conversion(line, hd_len_(seeds_num), seeds_num, changed);
            } else {
                assert(zeromem(changed, hd_len_(changed)) != 0);
                printf("%s\n", line);
            }
        }
    }

    i32 mini_seed = 0;
    for (int i = 1; i < hd_len_(seeds_num); ++i) {
        if (seeds_num[i] < seeds_num[mini_seed]) {
            mini_seed = i;
        }
    }
    printf("Minimum %dSeed: %lld (started as %s)\n", mini_seed, seeds_num[mini_seed], seeds_str[mini_seed]);
} 


i32 main(void) {
    A = new_arena(128*MBs_, mem);

    mstr *lines = file_to_lines(&A, "./txts/aoc.txt");
    clock_t start = clock();
    aoc(hd_len_(lines), lines);
    print_clock(start);

    return 0;
}
