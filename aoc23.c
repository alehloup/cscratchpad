#include <math.h>
#include "ale_io.h"

#define print(...) //printf(__VA_ARGS__)
#define printn(...) //print(__VA_ARGS__); printf("\n")

static u8 mem[128*MBs_] = {0};
static Arena A = {0, 0};

typedef struct node{i32 l; i32 r;}node;

#define printlabel(str_) print(" %c%c%c ", (str_)[0], (str_)[1], (str_)[2])

const i32 ZZZ = 909090;
const i32 AAA = 656565;

_pure i32 codify(ccstr label) {
    return label[2]*1 + label[1]*100 + label[0]*10000;
}

_fun b32 continues(i32 lenpositions, i32 positions[]) {
    for (int i = 0; i < lenpositions; ++i) {
        if (positions[i]%100 != 90) {
            return True;
        }
    }
    return False;
}

//AoC 8
_proc aoc(ci64 lineslen, mstr lines[1]) {
    static node graph[1048576];
    ccstr directions = lines[0];
    i64 steps = 0;
    NEW_VEC(&A, positions, i32);
    
    assert(zeromem((u8*)graph, 1048576));

    for (int iline = 1; iline < lineslen; ++iline) {
        ccstr line = lines[iline];
        i32 mpos = 0;
        if (is_empty_string(line)) {
            continue;
        }

        mpos = codify(&line[0]);
        graph[mpos].l = codify(&line[7]);
        graph[mpos].r = codify(&line[12]);

        if (mpos%100 == 65) {
            vec_append(positions, mpos);
        }

        printn("%d = (%d, %d)", mpos, graph[mpos].l, graph[mpos].r);
    }

    printn("\n0: %d", mypos);
    while (continues(hd_len_(positions), positions)) {
        for (int i = 0; directions[i] && mypos%100 != 90; ++i) {
            mypos = directions[i] == 'L' ? graph[mypos].l : graph[mypos].r;
            ++steps;
            printn("%lld: %d", steps, mypos);

            if (steps % 100000000 == 0) {
                printf (" . ");
            }
        }
    }
    printf("Steps: %lld", steps);
    
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
