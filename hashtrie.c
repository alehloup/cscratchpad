#include "aleh/aleh.h"

int main(void) {
    char arr[KB];
    arena a = arr2arena(arr);

    htint *ints = 0;
    
    htset(&ints, S("Alessandro"), 52, &a);
    htset(&ints, S("Sarah"), 69, &a);

    htint * resint = htget(&ints, S("Alessandro"));
    if (resint) {
        println(resint->value);
    }

    resint = htget(&ints, S("Sarah"));
    if (resint) {
        println(resint->value);
    }

    htset(&ints, S("Karol"), 69, &a);

    foreach(x, (htint *)arr, (a.beg - arr)/ssizeof(htint)) {
        print(x->key); printf(" "); println(x->value);
    }

    ssize_t base = (ssize_t)a.beg;
    
    htstring *strings = 0;

    htset(&strings, S("Alessandro"), S("Stamatto"), &a);
    htset(&strings, S("Sarah"), S("Sakamoto"), &a);

    foreach(x, (htstring *)base, ((ptrdiff_t)a.beg - base)/ssizeof(htstring)) {
        print(x->key); printf(" "); println(x->value);
    }

    return 0;
}
