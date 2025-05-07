/* 
    [REQUIRED] to define htrie_valtype before including this header
    
    hashtrie_ds.h implements:
    |htrie_prefix|lookup for ht|htrie_prefix| struct, |htrie_keytype| key -> |htrie_valtype| val
*/

/* REQUIRED*/

#ifndef htrie_valtype
    #error "htrie_ds.h requires you to define htrie_valtype before including"
#endif


/* PARAMETERS */

// the type of the KEY, defaults to str
#ifndef htrie_keytype
    #define htrie_keytype str
#endif

// prefix to function and struct names, defaults to valtype
#ifndef htrie_prefix
    #define htrie_prefix htrie_valtype
#endif


/* STRUCT NAMES */

#define htrie_struct macrocat(ht, htrie_prefix)


/* FUNCTION NAMES */

#define htrie_lookup_fn macrocat(htrie_prefix, lookup)


/* STRUCTS */

typedef struct htrie_struct htrie_struct;
struct htrie_struct {
    htrie_struct *child[4];
    htrie_keytype key;
    htrie_valtype value;
};


/* FUNCTIONS */

// If null is passed as arena then does lookup and might return null, otherwise returns value address
static inline htrie_valtype * htrie_lookup_fn(htrie_struct **node, htrie_keytype key, arena *a) {
    size_t seed = node ? (size_t)*node : 0;
    for (size_t h = hash64(key, seed); *node; h <<= 2) {
        if (equal(key, (*node)->key)) {
            return &(*node)->value;
        }

        node = &(*node)->child[h >> 62];
    }
    if (!a) return NULL;

    *node =  new(a, 1, htrie_struct);
    (*node)->key = key;

    return &(*node)->value;
}


/* UNDEFS */

// undef generic parameters
#undef htrie_valtype
#undef htrie_keytype
#undef htrie_prefix

// undef struct names
#undef htrie_struct

// undef function names
#undef htrie_lookup_fn
