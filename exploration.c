#include "ale.h"

struct lencount_t { size_t len; size_t count; };

static inline int ss_cmp(const void *a_void, const void *b_void) {
    const struct lenstr_t *a = (const struct lenstr_t *)a_void, *b = (const struct lenstr_t *)b_void;
    return lenstr_cmp_locale(*a, *b);
}

static inline int lens_cmp(const void *a_void, const void *b_void) {
    const struct lencount_t *a = (const struct lencount_t *)a_void, *b = (const struct lencount_t *)b_void;
    return (int)(b->count - a->count); /* reverse */
}

static struct lenstr_t cities[1<<15] = {{0,0}};

static struct lenstr_t lines[32000];

static struct lenstr_t cities_arr[1024];

static inline void run(void) {
    size_t filesize = 0, lines_len = 0, cities_len = 0, lens_count[128] = {0};
    char *contents = mmap_open("./measurements10k.txt", "r", &filesize);
    
    buffer_to_lines(contents, filesize, arrsizeof(lines), lines, &lines_len);

    /* Process measurements%SZ.txt putting each city name in the hash table, and each line len in lens_count */
    for (size_t i = 0; i < lines_len; ++i) {
        unsigned int pos = 0;
        struct lenstr_t ss = {0, 0};

        ss.len = char_pos_lenstr(';', lines[i]);
        ss.str = lines[i].str;

        pos = ht_lenstr_upsert(ss, arrsizeof(cities), cities, &cities_len);
        (void) pos;

        ++lens_count[lines[i].len];
    }

    { /* Print Cities Names sorted */
        size_t cities_arr_len = 0;

        ht_lenstr_to_arr(
            arrsizeof(cities), cities,
            arrsizeof(cities_arr), cities_arr, &cities_arr_len
        );

        qsort(cities_arr, cities_arr_len, sizeof(cities_arr[0]), ss_cmp);

        for (size_t i = 0; i < cities_arr_len; ++i) {
            lenstr_printend(cities_arr[i], "\n");
        }
    }

    { /* Show Counts of len of cities names */
        struct lencount_t counts[64] = {{0, 0}};
        size_t counts_len = 0;

        for (size_t i = 0; i < arrsizeof(lens_count); ++i) {
            if (lens_count[i]) {
                struct lencount_t cnts = {0, 0};
                cnts.count = i;
                cnts.len = lens_count[i];
                counts[counts_len++] = cnts;
            }
        }

        qsort(counts, counts_len, sizeof(counts[0]), lens_cmp);

        for (size_t i = 0; i < counts_len; ++i) {
            printf("%zu: %zu, ", counts[i].len, counts[i].count);
        }
        printf("\n");
    }

    mmap_close(contents, filesize);
}

int main(void) {
    set_locale_to_utf8();
    start_benchclock();
    run();
    stop_benchclock();
    return 0;
}
