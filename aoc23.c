#include <math.h>
#include "ale_io.h"

#define print(...)  //printf(__VA_ARGS__)
#define printn(...) //print(__VA_ARGS__); printf("\n")

static u8 mem[128*MBs_] = {0};
static Arena A = {0, 0};

typedef struct node{i32 l; i32 r;}node;

#define printlabel(str_) print(" %c%c%c ", (str_)[0], (str_)[1], (str_)[2])


_pure i32 codify(ccstr label) {
    return label[2]*1 + label[1]*100 + label[0]*10000;
}

//AoC 8
_proc aoc(ci64 lineslen, mstr lines[1]) {
    static node graph[1048576];
    ccstr directions = lines[0];
    ci32 lendirections = (i32) cstrlen(lines[0]);
    i32 lenpositions = 0;
    i64 lmc = 1;
    NEW_VEC(&A, positions, i32);
    NEW_VEC(&A, stepends, i64);
    
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
            vec_append(stepends, 0);
        }

        printn("%d = (%d, %d)", mpos, graph[mpos].l, graph[mpos].r);
    }

    lenpositions = hd_len_(positions);

    print("Starting positions: ");
    print_vec(positions, "%d ");

    
    for (int ipos = 0; ipos < lenpositions; ++ ipos) {
        i64 steps = 0;
        
        // each letter is encoded in ASCII: ZZZ is 909090, BBZ 666690, etc
        for (int i = 0; positions[ipos]%100 != 90; i = (i+1)%lendirections, ++steps ) {
            ci32 pos = positions[ipos];
            positions[ipos] = directions[i] == 'L' ? graph[pos].l : graph[pos].r;
        }
        stepends[ipos] = steps;

        print("%lld [%d:%d] ", steps, ipos, positions[ipos]);

    }
    printn(" ");

    for (int ipos = 0; ipos < lenpositions; ++ ipos) { 
        lmc = least_common_multiple(lmc, stepends[ipos]);
    }
    printf("Ghosts Arrive at: %lld", lmc);

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
