#include "ale.h"

#define NUM_THREADS 12 // Also works for 4096 threads > : )
static const int SINGLE_THREAD = NUM_THREADS == 1;
static const int MULTI_THREAD = NUM_THREADS > 1;

#define FILEPATH "./measurements1B.txt"


#define CITIES_CAP_ 2048
typedef struct City { struct sslice_t name; size_t count; long long int sum; long long int min; long long int max; } City;
static City thread_cities[NUM_THREADS][CITIES_CAP_] = ZERO_INIT_;
static struct sslice_t thread_hts[NUM_THREADS][CITIES_CAP_] = ZERO_INIT_;
static size_t thread_hts_lens[NUM_THREADS] = ZERO_INIT_;

static struct mmap_t *mmap_info = 0; // will store the mmaped file

fun_ City * my_cities(unsigned int thread_idx) {
    City* cities = thread_cities[thread_idx];

    for (unsigned int i = 0; i < CITIES_CAP_; ++i) {
        cities[i].min =  1000;
        cities[i].max = -1000;
    }

    return cities;
}

// Chunks the input by the thread_idx, runs in entire content if NUM_THREADS == 1
routine_ chunked_run(void* threadidx /* thread_idx */) {
    unsigned int thread_idx = (unsigned int)(size_t) threadidx;
    unsigned int line_num = 0;
    
    City *cities = my_cities(thread_idx);
    struct sslice_t *ht = thread_hts[thread_idx];
    size_t *ht_len = &thread_hts_lens[thread_idx];

    char *data = mmap_info->contents;

    /* Adjusts the start and end indexes based on thread_idx */
    size_t i = 0, len = mmap_info->filesize - 1;
    const char *cur = 0, *end = 0;

    if (MULTI_THREAD) {
        const size_t slice_size = (size_t) mmap_info->filesize / NUM_THREADS; // size per thread
        
        i = (slice_size * ((size_t) thread_idx));
        i = i > 0 ? i - 1 : 0;

        len = i + slice_size;

        while (data[len] != '\n') ++len;

        if (thread_idx == NUM_THREADS - 1) {
            len = mmap_info->filesize - 1; // last thread gets the excedent
        }
    }

    if (i > 0) {
        while (data[i] != '\n') 
            ++i;
        ++i; // skips to next line
    } else {
        i = 0;
    }

    assert(i < len);
    assert(len < mmap_info->filesize);

    cur = &data[i]; 
    end = &data[len];
    /* -----------------------------------------  */

    (void) line_num;

    while(cur < end) {
        unsigned int pos = 0; // will be used for indexing the ht
        struct sslice_t city_name = {.len=0, .text = cur};
        City *city = 0; long long int measure = 1;

        // Increment to move past the ';'
        while(*cur != ';') {
            ++cur;
        }
        city_name.len = (size_t)(cur - city_name.text);
        ++cur;

        //parse number
        measure = 1;
        if (cur[0] == '-') {
            measure = -1;
            ++cur;
        }
        if (cur[1] == '.') {
            measure *= (cur[0] - '0')*10 + (cur[2] - '0');
            cur += 4;
        } else {
            measure *= (cur[0] - '0')*100 + (cur[1] - '0')*10 + (cur[3] - '0');
            cur += 5;
        }

        //update city measurements
        pos = ht_sslice_upsert(city_name, CITIES_CAP_, ht, ht_len);

        city = &cities[pos];

        city->name = city_name;

        ++city->count;
        city->sum += measure;
        
        if (measure < city->min) {
            city->min = measure;

        }
        if (measure > city->max) {
            city->max = measure;
        }

        ++line_num;
    }

    return 0;
}

proc_ aggregate_results(void) {
    City* thread0 = thread_cities[0];

    for (unsigned int i_thread = 1; i_thread < NUM_THREADS; ++i_thread) {
        City* thread_cur = thread_cities[i_thread];

        for (unsigned int i = 0; i < ncity; ++i) {
            City *dst_city = &thread0[hash_order[i]];
            City *src_city = &thread_cur[hash_order[i]];

            dst_city->count += src_city->count;
            dst_city->sum   += src_city->sum;
            
            if (src_city->min < dst_city->min) {
                dst_city->min = src_city->min;
            }
            if (src_city->max > dst_city->max) {
                dst_city->max = src_city->max;
            }
        }
    }
}

proc_ print_results(void) {
    for (unsigned int i = 0; i < ncity; ++i) {
        City *city = &thread_cities[0][hash_order[i]];
        printf("%s  #%zu  $%.1f  @%.1f  [%.1f  %.1f]\n", city->name, city->count, 
            /*$sum*/ (double)city->sum / 10.0,
            /*@avg*/ ((double)city->sum / ((double)city->count*10)),
            /*[min  max]*/ (double)city->min/10.0, (double)city->max/10.0
        );
    }
}

proc_ run(void) {
    int printed = printf("\n Running 1BRC on file: %s\n", FILEPATH);
    struct mmap_t mmap_info_local = mmap_open(FILEPATH);
    mmap_info = &mmap_info_local;

    (void) printed;
    
    if (SINGLE_THREAD) {
        chunked_run(0);
    } else {
        THREAD_T threads[NUM_THREADS];
        size_t threads_len = 0;

        go_threads( chunked_run, NUM_THREADS, CAP_(threads), threads, &threads_len);
        join_threads(threads, threads_len);
        aggregate_results();
    }

    print_results();
}

int main(void) {

    start_benchclock();
    run();
    stop_benchclock();

    return 0;
}
