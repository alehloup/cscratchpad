#include "ale.h"

#define N_LINES 100000ll
#define N_LINES_STR "100k"

proc_ read_cidades(const int64_t cap, struct sslice_t cidades[], int64_t *len) {
    struct mmap_t m = mmap_open("cidades.txt");
    buffer_to_lines(m.contents, m.filesize, cap, cidades, len);
} 

proc_ run(void) {
    struct sslice_t cidades[512] = ZERO_INIT_; 
    int64_t cidades_len = 0;
    read_cidades(CAP_(cidades), cidades, &cidades_len);

    file_create("measurements" N_LINES_STR ".txt", 16ll * N_LINES);
    
    struct mmap_t m = mmap_open_for_write("measurements" N_LINES_STR ".txt");
    char *buffer = m.contents;
    int64_t idx = 0;

    uint64_t seed = 41635984;
    for (int i = 0; i < N_LINES; ++i) {
        int printit = i % 10000000 == 0;

        struct sslice_t cidade = cidades[rnd(&seed) % cidades_len];
        buffer_appendslice(m.filesize, buffer, &idx, cidade);
        buffer_appendcstr(m.filesize, buffer, &idx, ";");

        if (printit) {
            printf("[%d] ", i);
            sslice_printend(cidade, ";");
        }

        int64_t start_idx = idx;

        int64_t measurement = rnd(&seed) % 1001;

        if (rnd(&seed) % 2 == 1) {
            buffer[idx++] = '-';
        }

        int64_t tenths = measurement / 100;
        int64_t unities = measurement;
        if (tenths > 0) {
            buffer[idx++] = '0' + (char)tenths;
            unities = unities % 100;
        }
        buffer[idx++] = '0' + (char)(unities / 10);
        buffer[idx++] = '.';
        buffer[idx++] = '0' + (char)(unities % 10);

        buffer[idx++] = '\n';

        if (printit) {
            printf("%.*s", (int32_t)(idx - start_idx), &buffer[start_idx]);
        }
    }
    printf("size: \n%lld\n", idx);

    for (int64_t i = idx; i < m.filesize; ++i) {
        buffer[i] = '_';
    }
    mmap_close(m);

    file_truncate("measurements" N_LINES_STR ".txt", idx);
}

int main(void) {
    run();
    printf("Done \n");
}
