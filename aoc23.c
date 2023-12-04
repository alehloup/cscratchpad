#include "ale_io.h"

_fun b32 aoc3_is_symbol(cchar letter) {
    return letter != '\0' && (letter < '.' || letter > '9' || letter == '/');
}

_proc_rbuffer(1, 2)
aoc3(i32 lineslen, cstr lines[]) {
    i64 sum = 0;

    for (int iline = 0; iline < lineslen; ++iline) {
        cstr line = lines[iline];
        for (i64 iletter = 0; line[iletter]; ++iletter) {
            if(is_digit(line[iletter])) {
                i64num_i64len num_len = cstr_to_num(&line[iletter]);
                b32 touches_symbol = False;
                ci64 start = iletter > 0 ? iletter - 1 : iletter;
                ci64 end = iletter+num_len.len + 1; //lines always end with '\0'

                if (iline > 0) {
                    cstr top = lines[iline - 1];
                    for (i64 itop = start; !touches_symbol && itop < end && top[itop]; ++itop)
                        touches_symbol |= aoc3_is_symbol(top[itop]);
                }
                //sides:
                touches_symbol |= iletter > 0 && aoc3_is_symbol(line[iletter-1]);
                touches_symbol |= aoc3_is_symbol(line[iletter+num_len.len]);
                    
                if (iline < lineslen - 1) {
                    cstr bot = lines[iline + 1];
                    for (i64 ibot = start; !touches_symbol && ibot < end && bot[ibot]; ++ibot)
                        touches_symbol |= aoc3_is_symbol(bot[ibot]);
                }

                printf("%lld#%lld (%d), ", num_len.num, num_len.len, touches_symbol);
                
                iletter += num_len.len;
                sum += touches_symbol ? num_len.num : 0;
            }
        }
    }
    printf("\n Sum: %lld \n", sum);
}

_proc_rbuffer(1, 2)
aoc2(i32 lineslen, mstr lines[]) {
    static u8 buff[1*MBs_] = {0};
    Arena arena = newarena(1*MBs_, buff);
    
    i8 RED = (i8)'r'-'b', GREEN = (i8)'g'-'b', BLUE = (i8)'b'-'b';
    i64 powersum = 0;
    for (int iline = 0; iline < lineslen; ++iline) {
        i32 game_id = iline + 1, power = 0, maxis[17] = {0};

        Vec64 game = mutslice_by_splitter(&arena,lines[iline],':');
        Vec64 grabs = mutslice_by_splitter(&arena,(mstr)game.data[1],';');

        printf("     GAME %d    \n", game_id);

        for (int igrab = 0; igrab < grabs.len; ++igrab) {
            Vec64 cubes = mutslice_by_splitter(&arena,  
                (mstr) grabs.data[igrab], ',');

            for (int icube = 0; icube < cubes.len; ++ icube) {
                mstr cube = (mstr) cubes.data[icube];
                int n = 0; char color = 'n';
                i8 coloridx = 0;

                sscanf_s(cube, " %d %c", &n, &color, 1);
                printf("%d%c ", n, color);

                coloridx = (i8)color-'b';
                maxis[coloridx] = n > maxis[coloridx] ? n : maxis[coloridx];
            }
            printf("\n");
        }
        powersum += power = maxis[RED]*maxis[GREEN]*maxis[BLUE];

        printf("needed: %d red %d green %d blue (%d)\n\n", maxis[RED],maxis[GREEN],maxis[BLUE], power);
    }
    printf("Powersum: %lld", powersum);

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
    cstr *data = vec_data_as_cstr(&lines);

    aoc3(lines.len, data);

    return 0;
}
