#include "ale.h"

proc_ run(void) {
    struct mmap_t m = mmap_open("measurements1b.txt");

    struct sslice_t text = {m.filesize, m.contents};
    struct sslice_t line = ZERO_INIT_;

    int64_t i = 0, j = 0;
    while ( (line = next_line(&text)).len > 0 ) {
        if (i % 1000000 == 0 or i > 999999994) {
            ++j;
        }

        ++i;
    }

    printf("%lld\n", j);
}

BENCH_MAIN_