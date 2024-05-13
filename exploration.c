#include "ale.h"

struct len_count_t { size_t len; size_t count; };

fun_ int ss_cmp(const struct sslice_t *a, const struct sslice_t *b) {
    return sslice_cmp_locale(*a, *b);
}

fun_ int lens_cmp(const struct len_count_t *a, const struct len_count_t *b) {
    return (int)(b->count - a->count); // reverse
}

static struct sslice_t cities[1<<15] = {{0,0}};

static struct sslice_t lines[32000];

static struct sslice_t cities_arr[1024];

proc_ run(void) {
    struct mmap_t map = mmap_open("./measurements10k.txt");
    size_t lines_len = 0, cities_len = 0, lens_count[128] = {0};
    
    buffer_to_lines(map.contents, map.filesize, CAP_(lines), lines, &lines_len);

    // Process measurements%SZ.txt putting each city name in the hash table, and each line len in lens_count
    for (size_t i = 0; i < lines_len; ++i) {
        struct sslice_t ss = {char_pos_slice(';', lines[i]), lines[i].text };
        unsigned int pos = ht_sslice_upsert(ss, CAP_(cities), cities, &cities_len);
        (void) pos;

        ++lens_count[lines[i].len];
    }

    { // Print Cities Names sorted 
        size_t cities_arr_len = 0;

        ht_sslice_to_arr(
            CAP_(cities), cities,
            CAP_(cities_arr), cities_arr, &cities_arr_len
        );

        qsort(cities_arr, cities_arr_len, sizeof(cities_arr[0]), (cmp_fun_t)ss_cmp);

        for (size_t i = 0; i < cities_arr_len; ++i) {
            sslice_printend(cities_arr[i], "\n");
        }
    }

    { // Show Counts of len of cities names
        struct len_count_t counts[64] = {{0, 0}};
        size_t counts_len = 0;

        for (size_t i = 0; i < CAP_(lens_count); ++i) {
            if (lens_count[i]) {
                struct len_count_t cnts = {i, lens_count[i]};
                counts[counts_len++] = cnts;
            }
        }

        qsort(counts, counts_len, sizeof(counts[0]), (cmp_fun_t)lens_cmp);

        for (size_t i = 0; i < counts_len; ++i) {
            printf("%zu: %zu, ", counts[i].len, counts[i].count);
        }
        printf("\n");
    }

    mmap_close(map);
}

int main(void) {
    set_locale_to_utf8();
    start_benchclock();
    run();
    stop_benchclock();
    return 0;
}
