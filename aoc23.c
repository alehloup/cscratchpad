#include "ale_io.h"

#define print(...) 

static u8 mem[128*MBs_] = {0};
static Arena A = {0, 0};

typedef struct range{i64 start; i64 end;}range;

_proc apply_conversion(ccstr line, i32 lenseeds, range seeds[20], u8 changed[20]) {
    i64 dst, src, len;
    sscanf_s(line, "%lld %lld %lld", &dst, &src, &len);

    ci64 addit = dst - src,
         src_start = src, src_end = src + len;

    print("===(%lld - %lld|+%lld {%lld->%lld})===\n", dst, src, addit, src_start, src_end);

    for (int iseed = 0; iseed < lenseeds; ++iseed) {
        i64 seed_start = seeds[iseed].start, seed_end = seeds[iseed].end;

        if (!changed[iseed] && seed_start < src_end && seed_end > src_start) {
            range newseed = {0, 0};
            newseed.start = (seed_start >= src_start ? seed_start : src_start);
            newseed.end = (seed_end <= src_end ? seed_end : src_end);

            print("%d#%lld->%lld sliced to", iseed, seed_start, seed_end);

            if (seed_start < newseed.start && seed_start < newseed.end) {
                range beforeseed = {0, 0};
                beforeseed.start = seed_start;
                beforeseed.end = newseed.start;

                print(" #%d|%lld->%lld| <", hd_len_(seeds), beforeseed.start, beforeseed.end);
                if (beforeseed.start < beforeseed.end) {
                    vec_append(seeds, beforeseed);
                    vec_append(changed, False);
                } else {
                    print(" !! BEFORE START IS NOT LESSER THAN BEFORE END !! ");
                }
            }

            print(" #%d\\|%lld->%lld|/", iseed, newseed.start, newseed.end);
            if (newseed.start >= newseed.end) {
                print(" !! SLICED START IS NOT LESSER THAN BEFORE END !! ");
            }
            

            if (seed_end > newseed.end && seed_end > newseed.start) {
                range afterseed = {0, 0};
                afterseed.start = newseed.end;
                afterseed.end = seed_end;

                print(" < #%d|%lld->%lld|", hd_len_(seeds), afterseed.start, afterseed.end);
                if (afterseed.start < afterseed.end) {
                    vec_append(seeds, afterseed);
                    vec_append(changed, False);
                } else {
                    print(" !! AFTER START IS NOT LESSER THAN BEFORE END !! ");
                }
            }
            
            newseed.start += addit;
            newseed.end += addit;
            print(" and converted to #%d{%lld->%lld} \n", iseed, newseed.start, newseed.end);
            seeds[iseed] = newseed;
            changed[iseed] = True;
        }
    }
}

//AoC 5
_proc aoc(ci32 lineslen, mstr lines[1]) {
    mstr seeds = split(&A, lines[0], ':')[1];
    mstr *seeds_str = split(&A, seeds, ' ');
    range *seeds_ranged = NEW_VEC_WITH_CAP(&A, range, 256);
    u8 *changed = NEW_VEC_WITH_CAP(&A, u8, 256);
    i32 mini_seed = 0;

    for (int i = 0; i < hd_len_(seeds_str); i += 2) {
        range seed = {0, 0};
        seed.start = cstr_to_num(seeds_str[i]).num;
        seed.end   = seed.start + cstr_to_num(seeds_str[i+1]).num;
        vec_append(seeds_ranged, seed);
        vec_append(changed, False);
    }
    assert(hd_len_(seeds_str) / 2 == hd_len_(seeds_ranged));

    print(" SEEDS: ");
    for(int i = 0; i < hd_len_(seeds_ranged); ++i) {
        print("\\%lld->%lld/ ", seeds_ranged[i].start, seeds_ranged[i].end);
    }

    for (int iline = 2; iline < lineslen; ++iline) {
        ccstr line = lines[iline];
        if (!is_empty_string(line)) {
            if (is_digit(line[0])) {
                apply_conversion(line, hd_len_(seeds_ranged), seeds_ranged, changed);
            } else {
                assert(zeromem(changed, hd_len_(changed)) != 0);
                print("\n\n");
                print(" SEEDS: ");
                for(int i = 0; i < hd_len_(seeds_ranged); ++i) {
                    print("\\%lld->%lld/ ", seeds_ranged[i].start, seeds_ranged[i].end);
                }
                print("\n%s\n", line);
            }
        }
    }

    print("\n\nSeeds: ");
    for (int i = 1; i < hd_len_(seeds_ranged); ++i) {
        print("\\%lld->%lld/ ", seeds_ranged[i].start, seeds_ranged[i].end);
        if (seeds_ranged[i].start < seeds_ranged[mini_seed].start) {
            mini_seed = i;
        }
    }
    print("\n");
    printf("\nMinimum #%dSeed: %lld \n", mini_seed, seeds_ranged[mini_seed].start);
} 


i32 main(void) {
    A = new_arena(128*MBs_, mem);

    mstr *lines = file_to_lines(&A, "./txts/aoc.txt");
    clock_t start = clock();
    aoc(hd_len_(lines), lines);
    print_clock(start);

    return 0;
}
