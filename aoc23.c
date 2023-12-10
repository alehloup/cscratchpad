#include <math.h>
#include "ale_io.h"

#define print(...) 

static u8 mem[128*MBs_] = {0};
static Arena A = {0, 0};

//card strength
static i32 cs[128] = {0};
_proc setStrengths(i32 cs[128]) {
    cs['2'] = 2;  cs['3'] = 3;  cs['4'] = 4;  cs['5'] = 5;
    cs['6'] = 6;  cs['7'] = 7;  cs['8'] = 8;  cs['9'] = 9;
    cs['T'] = 10; cs['J'] = 11; cs['Q'] = 12; cs['K'] = 13;
    cs['A'] = 14;
}

_fun i32 type_score(const char hand[5]) {
    i8 equals[5] = {0, 0, 0, 0, 0};
    i32 sum = 0;

    
    for (int cur = 0; cur < 5; ++ cur) {    
        for (int i = 0; i < 5; ++i) {
            if (hand[cur] == hand[i]) {
                equals[cur] += 1;
            }
        }
    }
    for (int i = 0; i < 5; ++i) {
        sum += equals[i];
    }

    return sum;
}

_fun i32 card_compare(cvoidp cardAref, cvoidp cardBref) {
    cstr cardA = *((cstr*)cardAref), cardB = *((cstr*)cardBref);
    i32 type = type_score(cardA) - type_score(cardB);
    
    if (type) {
        return type;
    }

    for (int i = 0; i < 5; ++i) {
        ci32 untie = cs[(i32)cardA[i]] - cs[(i32)cardB[i]];
        if (untie) {
            return untie;
        }
    }

    return 0;
}

//AoC 7
_proc aoc(ci64 lineslen, mstr lines[1]) {
    i64 total = 0;
    setStrengths(cs);

    vec_custom_sort_cstr(lines, card_compare);

    for (int i = 0; i < lineslen; i++) {
        ci32 rank = i + 1;
        i32 hand_wins = 0, wins = 0;
        sscanf_s(&lines[i][6], "%d", &hand_wins);
        wins = rank*hand_wins;
        
        total += wins;
        print("%s |rank(%d) * bid{%d} = %d  curSum#%lld\n", lines[i], rank, hand_wins, wins, total);
    }
    printf("\nTotal Winnings: %lld", total);
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
