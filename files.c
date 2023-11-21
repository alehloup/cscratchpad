#include "ale.h"

static thread_local uint8_t buffer[2*_Mega_Bytes];

int main(void) {
    arena_t perm = newarena(sizeof(buffer), buffer);
    
    s8 contents = file_to_buffer(&perm, "exemplo.txt");
    printf("%s", contents.data);    
}
