#include <math.h>
#include "ale_io.h"

#define print(...)  //printf(__VA_ARGS__)
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
    ci32 lendirections = (i32) cstrlen(lines[0]);
    i32 lenpositions = 0;
    i64 steps = 0, attempt = 0;
    NEW_VEC(&A, positions, i32);
    i32 iprevious = 0, pos_to_attempt = 0;
    
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

    lenpositions = hd_len_(positions);

    print("Starting positions: ");
    print_vec(positions, "%d ");

    while (continues(lenpositions, positions)) {
        i64 cycle_steps = 0;
        i32 idir = iprevious;

        ++attempt;

        do {
            ci32 pos = positions[pos_to_attempt];
            positions[pos_to_attempt] = directions[idir] == 'L' ? graph[pos].l : graph[pos].r;
            ++cycle_steps;

            idir = (idir + 1)%lendirections;
        } while (positions[pos_to_attempt]%100 != 90);

        steps += cycle_steps;

        for (int ipos = 0; ipos < hd_len_(positions); ++ ipos) {
            int keepup_steps = 0;

            if (ipos == pos_to_attempt) {
                print("[%d:%d] ", ipos, positions[ipos]);
                continue;
            }
            
            for (int i = iprevious; directions[i] && keepup_steps < cycle_steps; ++i) {
                ci32 pos = positions[ipos];
                positions[ipos] = directions[i] == 'L' ? graph[pos].l : graph[pos].r;
                ++keepup_steps;
            }
            
            print("[%d:%d] ", ipos, positions[ipos]);
        }
        printn(" ");

        if (attempt % 100000 == 0) {
            printf("\n#%lld\n", steps);
            print_vec(positions, "%d ");            
        }
        

        iprevious = idir;
        pos_to_attempt = (pos_to_attempt+1)%lenpositions;
    }
    printf("%lld \n", steps);
    print_vec(positions, "%d ");    
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
