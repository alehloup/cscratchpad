#include <stdio.h>
#include <windows.h>
#include <time.h>
#include "ale.h"

_proc run(void) {
    Long cities_counts[12789] = ZERO_INIT;
    _new_array(cities_names, String, 512);

    Mmap mmap_info = mmap_open(S("C:/Users/Aleh/1brc_java/measurements1b.txt")); 
    {
        Buffer contents = mmap_info.contents;
        Long len = contents.len;
        Byte *data = contents.elements;

        Long i = 0;
        while (i < len) {
            Long base_idx = i;
            _new_array(city_name, Byte, 32);

            while(data[i] != ';') {
                _append(&city_name, data[i++]);
            }
            city_name.elements[city_name.len] = 0;

            ULong city_hash = (*((ULong*)(&city_name.elements[1])));
            city_hash %= 12789;

            //printf("%.*s %llu\n", (Int)city_name.len, &data[base_idx], city_hash);

            if (cities_counts[city_hash] == 0) {
                String city = {city_name.len, &data[base_idx]};
                _append(&cities_names, city);
            }
            ++cities_counts[city_hash];
            

            while(data[i] != '\n') 
                ++i; 

            ++i;
        }
        printf("#City: %lld\n", cities_names.len);
        for(Long i = 0; i < cities_names.len; ++i) {
            //string_print(cities_names.elements[i]); printf(", ");
        }
        printn;
    }
    mmap_close(mmap_info);
}

int main(void) {

    START_WATCH;
    run();
    STOP_WATCH;

    return 0;
}
