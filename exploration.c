#include "ale.h"

static struct sslice_t cities[1<<15] = ZERO_INIT_;

NEWCMPF_(ss_cmp, struct sslice_t, return sslice_cmp_locale(a, b))

proc_ run(void) {
    struct mmap_file_t map = mmap_open("./measurements10k.txt");
    struct sslice_t lines[32000];
    int64_t lines_len = 0;

    buffer_to_lines(map.contents, map.filesize, ARRCAP_(lines), lines, &lines_len);

    int64_t cities_len = 0;

    for (int64_t i = 0; i < lines_len; ++i) {
        struct sslice_t ss = {char_pos_slice(';', lines[i]), lines[i].text };
        int32_t pos = ht_sslice_upsert(ss, ARRCAP_(cities), cities, &cities_len);
        (void) pos;
    }

    struct sslice_t cities_arr[1024];
    int64_t cities_arr_len = 0;

    ht_sslice_to_arr(
        ARRCAP_(cities), cities,
        ARRCAP_(cities_arr), cities_arr, &cities_arr_len
    );

    qsort(cities_arr, (size_t)cities_arr_len, sizeof(cities_arr[0]), ss_cmp);

    for (int i = 0; i < cities_arr_len; ++i) {
        sslice_print(cities_arr[i]);
    }
}

int main(void) {
    set_locale_to_utf8();
    start_benchclock();
    run();
    stop_benchclock();
    return 0;
}
