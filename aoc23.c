#include <math.h>
#include "ale_io.h"

#define print(...) //printf(__VA_ARGS__)

static u8 mem[128*MBs_] = {0};
static Arena A = {0, 0};

ci32 
    FIVE  = 25, 
    FOUR  = 17,
    FULL  = 13,
    THREE = 11,
    DOUBLE = 9,
    PAIR = 7,
    ONES = 5
;
_fun i32 adjust_for_jokers(ci32 score, ci32 njokers) {
    if (!njokers) {
        return score;
    } else if (score >= FULL) {
        return FIVE;
    } else if (score == THREE) {
        return FOUR;
    } else if (score == DOUBLE) {
        return njokers == 1 ? FULL : FOUR;
    } else if (score == PAIR) {
        return THREE;
    } else {
        return PAIR;
    }
}

_fun b32 has_joker(cchar hand[5]) {
    for (int i = 0; i < 5; ++i) {
        if (hand[i] == 'J')
            return True;
    }
    return False;
}

//card strength
static i32 cs[128] = {0};
_proc setStrengths(i32 cs[128]) {
    cs['J'] = 1;
    cs['2'] = 2;  cs['3'] = 3;  cs['4'] = 4;  cs['5'] = 5;
    cs['6'] = 6;  cs['7'] = 7;  cs['8'] = 8;  cs['9'] = 9;
    cs['T'] = 10; cs['Q'] = 11; cs['K'] = 12; cs['A'] = 13;
}

_fun i32 type_score(cchar hand[5]) {
    i8 equals[5] = {0, 0, 0, 0, 0};
    i32 sum = 0, js = 0;
    
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

    for (int i = 0; i < 5; ++i) {
        js += hand[i] == 'J';
    }

    return adjust_for_jokers(sum, js);
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
        print("%s | =%d= rank(%d)  \n", lines[i], type_score(lines[i]), rank);
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
