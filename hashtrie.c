#include "aleh/aleh.h"

int main(void) {
    char arr[KB];
    arena a = arr2arena(arr);

    htint *ints = 0;
    
    *intlookup(&ints, S("Alessandro"), &a) = 52;
    *intlookup(&ints, S("Sarah"), &a) = 69;

    int * resint = intlookup(&ints, S("Alessandro"), 0);
    if (resint) {
        println(*resint);
    }

    resint = intlookup(&ints, S("Sarah"), 0);
    if (resint) {
        println(*resint);
    }

    *intlookup(&ints, S("Karol"), &a) = 69;

    forspan(x, (htint *)arr, (htint *)a.beg) {
        print(x->key); printf(" "); println(x->value);
    }

    ssize_t base = (ssize_t)a.beg;
    
    htstr *strings = 0;

    *strlookup(&strings, S("Alessandro"), &a) = S("Stamatto");
    *strlookup(&strings, S("Sarah"), &a) = S("Sakamoto");

    forspan(x, (htstr *)base, (htstr *)a.beg) {
        print(x->key); printf(" "); println(x->value);
    }

    return 0;
}
