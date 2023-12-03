#include "ale_io.h"

_proc_rbuffer(1, 2)
aoc3(i32 lineslen, mstr lines[]) {
    static u8 buff[1*MBs_] = {0};
    Arena arena = newarena(1*MBs_, buff);
    i32 possibles = 0;
    i8 RED = 0, GREEN = 1, BLUE = 2, totals[3] = {0};
    totals[RED] = 12; totals[GREEN] = 13; totals[BLUE] = 14;
    
    printf("total_reds: %d, total_greens: %d, total_blues: %d\n", totals[RED], totals[GREEN], totals[BLUE]);

    for (int iline = 0; iline < lineslen; ++iline) {
        i32 game_id = iline + 1;
        Vec64 game = mutslice_by_splitter(&arena,  lines[iline], ':');
        Vec64 grabs = mutslice_by_splitter(&arena,  (mstr) game.data[1], ';');
        b32 possible_game = True;

        printf(" === GAME %d ===\n", iline + 1);

        for (int igrab = 0; igrab < grabs.len; ++igrab) {
            Vec64 cubes = mutslice_by_splitter(&arena,  (mstr) grabs.data[igrab], ',');

            for (int icube = 0; icube < cubes.len; ++ icube) {
                mstr cube = (mstr) cubes.data[icube];
                int n = 0;
                char color[8] = {0};
                b32 possible_cube = True;

                sscanf_s(cube, " %d %s", &n, color, 8);
                printf("|%dx%s", n, color);

                switch (color[0])
                {
                    case 'r': possible_cube = n <= totals[RED];  break;
                    case 'g': possible_cube = n <= totals[GREEN];  break;
                    case 'b': possible_cube = n <= totals[BLUE];  break;
                    default: printf("\nERROR PARSING!\n");
                }

                if (!possible_cube) {
                    printf(" <IMPOSSIBLE> ");
                    possible_game = False;
                }
            }
            printf("|\n");
        }
        
        if (!possible_game) {
            printf(" IMPOSSIBLE GAME! \n");
        } else {
            possibles += game_id;
        }

        printf(" ================= \n");
    }

    printf("Possibles: %d\n", possibles);

}

_proc_rbuffer(1, 2)
aoc1(i32 lineslen, cstr lines[]) {
    i64 sum = 0;
    for (int iline = 0; iline < lineslen; ++iline) {
        cstr line = lines[iline];
        i8 first = -1, last = -1;

        if (is_empty_string(line)) {
            continue;
        }
        
        for (int i = 0; line[i]; ++i) {
            i8 digit = named_digit(&line[i]);
            if (digit > -1) {
                if (first == -1) {
                    first = digit;
                } 
                last = digit;
            }
        }
        printf("%s: %d%d\n", line, first, last);
        sum += first*10 + last;
    }    
    printf("Sum: %lld\n", sum);
}

i32 main(void) {
    static u8 mem[128*MBs_] = {0};
    Arena arena = newarena(128*MBs_, mem);

    Vec64 lines = file_to_nonempty_lines(&arena, "aoc.txt");
    mstr *data = vec_data_as_mstr(&lines);

    aoc3(lines.len, data);

    return 0;
}
