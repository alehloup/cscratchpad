#include <math.h>
#include "ale_io.h"

//#define print(...) 

static u8 mem[128*MBs_] = {0};
static Arena A = {0, 0};

_math_hot i64 distance(i64 hold, i64 race_ms) {
    return hold * (race_ms - hold);
}

//AoC 6
_proc aoc(ci64 lineslen, mstr lines[1]) {
    i64 product = 1;

    mstr line_times = split(&A, lines[0], ':')[1];
    mstr *times_str = split(&A, line_times, ' ');
    mstr line_distances = split(&A, lines[1], ':')[1];
    mstr *distances_str = split(&A, line_distances, ' ');
    
    i64 *times = NEW_VEC(&A, i64), *distances = NEW_VEC(&A, i64);
    for (int i = 0; i < hd_len_(times_str); ++i) {
        sscanf_s(times_str[i], "%lld", vec_inc_ref(times));
    }
    for (int i = 0; i < hd_len_(distances_str); ++i) {
        sscanf_s(distances_str[i], "%lld", vec_inc_ref(distances));
    }

    print_vec(times, "%lld, ");
    print_vec(distances, "%lld, ");
    printf("\n");

    for (int race = 0; race < hd_len_(times); ++race) {
        ci64 race_time = times[race], 
             race_record = distances[race];
        ci64 hold = (race_time/2) + race_time % 2; 
        ci64 time_traveled = race_time - hold;
        ci64 distance_traveled = hold * time_traveled;
        
        i64 possibilities = 1, sub = 1 + race_time % 2;
             
        printf("%d#Race, time: %lld, record: %lld\n", race+1, race_time, race_record);
        for (i64 dist = distance_traveled; dist >= race_record; dist -= (sub += 2)) {
            ++possibilities;
        }
        possibilities *= 2;
        possibilities -= !(race_time % 2);
        printf("Possibilities: %lld\n", possibilities);
        printf("\n\n");

        product *= possibilities;
    }
    printf("Product: %lld\n", product);

    discard_ lineslen;
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
