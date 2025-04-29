#include "aleh/aleh.h"

typedef const char * cstr;
#define htrie_keytype cstr
#define htrie_valtype cstr
#include "aleh/htrie_ds.h"

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

    forspan(cs, (htcstr *)arr, (htcstr *)a.beg) {
        print(cs->key); print(" "); println(cs->value);
    }

    return 0;
}
