#include "ale.h"

typedef const char * cstr;
decl_htrie(cstr, cstr, cstr);

int main(void) {
    char arr[KB] = {0};
    arena a = arr2arena(arr);

    println("Testando");

    char woa[16] = "Testando2";
    println(woa);

    char woa2[] = {'O', 'l', 'a', '\0'};
    println(woa2);

    print("Alessandro == Alessandro ? "); println(equal("Alessandro", "Alessandro"));

    htcstr * ht = 0;
    *cstrlookup(&ht, "Alessandro", &a) = "Stamatto";
    *cstrlookup(&ht, "Sarah", &a) = "Sakamoto";

    forspan(cs, (htcstr *)(uintptr_t)arr, (htcstr *)(uintptr_t)a.beg) {
        print(cs->key); print(" "); println(cs->value);
    }

    return 0;
}
