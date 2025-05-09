#include "ale.h"

typedef const char * cstr;

decl_cmpfn(comp, int, *a - *b)
decl_cmpfn(comp2, cstr, cstrcmp(*a, *b))

int main(void)
{
    int arr[] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

    println("Testando 3");

    sort(arr, countof(arr), comp);

    printarr(arr, countof(arr));

    cstr arr2[4] = { "Sarah", "Alessandro", "Karol", "Amanda" };
    
    sort(arr2, countof(arr2), comp2);

    printarr(arr2, countof(arr2));

    println(parseint(S("12345")));

    println(parsefloat(S("52.52")));

    str arr3[5] = {0};
    ssize_t len = split(S("Alessandro,Sarah, Karol, Amanda ,Brenda"), ',', arr3, countof(arr3));
    printarr(arr3, len);

    for(FILE *file = fopen("measurements10k.txt", "rb"); file; fclose(file), file = NULL) {
        println((double)filelen(file) / (double)KB);
    }

    int c = 10;
    for(MMAP content = mopen("measurements10k.txt", "r"); content.len; mclose(content), content.len = 0) {
        forlines(line, content) {
            println(line);
            if (!c--) break;
        } 
    }

    size_t seed = 52;
    unsigned int counts[21] = {0};
    for (int i = 0; i < 1000000; ++i) {
        counts[rando(&seed, 1, 20)] += 1;
    }
    printarr(counts, countof(counts));

    return 0;
}
