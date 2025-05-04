#include "aleh/aleh.h"

typedef const char * cstr;

decl_cmpfn(comp, int, *a - *b)
decl_cmpfn(comp2, cstr, cstrcmp(*a, *b))

int main(void) {
    int arr[] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

    println("Testando 3");

    sort(arr, countof(arr), comp);

    printarr(arr, countof(arr));

    cstr arr2[4] = { "Sarah", "Alessandro", "Karol", "Amanda" };
    
    sort(arr2, countof(arr2), comp2);

    printarr(arr2, countof(arr2));

    println(parseint(S("12345")));

    println(parsefloat(S("52.52")));

    return 0;
}
