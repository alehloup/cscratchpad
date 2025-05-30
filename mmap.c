#include "ale.h"

typedef const char * cstr;

decl_cmpfn(comp, int, *a - *b)
decl_cmpfn(comp2, cstr, cstrcmp(*a, *b))

int main(void)
{
    int arr[] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    cstr arr2[4] = { "Sarah", "Alessandro", "Karol", "Amanda" };

    str arr3[5] = {0};
    size_t len = split(S("Alessandro,Sarah, Karol, Amanda ,Brenda"), ',', arr3, countof(arr3));

    int c = 10;

    size_t seed = 52;
    unsigned int counts[21] = {0};

    println("Testando 3");

    sort(arr, countof(arr), comp);

    printarr("%d", arr, countof(arr));
    
    sort(arr2, countof(arr2), comp2);

    printarr("%s", arr2, countof(arr2));

    print_int(parseint(S("12345"))); printf("\n");

    print_float(parsefloat(S("52.52"))); printf("\n");

    printstrarr(arr3, len);

    for(FILE *file = fopen("measurements10k.txt", "rb"); file; fclose(file), file = 0) {
        print_ssize(filelen(file) / KB); printf("KBs \n");
    }

    for(str content = mopen("measurements10k.txt", "r"); content.len; mclose(content), content.len = 0) {
        forlines(line, content) {
            print_str(line); printf("\n");
            if (!c--) break;
        } 
    }

    for (int i = 0; i < 1000000; ++i) {
        counts[rando(&seed, 1, 20)] += 1;
    }
    printarr("%u", counts, countof(counts));

    return 0;
}
