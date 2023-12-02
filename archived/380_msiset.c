#include <stdio.h>
#include "ale.h"

typedef struct {
    ht32_t table;
    vector32_t cache;
    arena_t * arena;
} RandomizedSet;


_proc print_cache(RandomizedSet* obj) {
    printf("|Cache . .");
    for(i32 i = 0; i < obj->cache.len; ++i) {
        printf(" %d:%d ", i, obj->cache.data[i]);
    }
    printf("| ");
}

_fun RandomizedSet * randomizedSetCreate(void) {
    static u8 memory[4*MBs_] = {0};
    static arena_t arena = {0, 0};

    arena = newarena(sizeof(memory), memory);
    
    {
        RandomizedSet *S = (RandomizedSet *) alloc(&arena, sizeof(RandomizedSet), 1);
        S->arena = &arena;

        S->table = new_ht32(&arena, 200000);
        vec_push_i32(&S->cache, &arena, 0);
        return S;
    }
}

_fun b32 randomizedSetInsert(RandomizedSet* obj, i32 val) {
    i32 msiidx = 0;
    entry32_t entry = {0, 0};

    b32 ret = False;
    if (val == 0) {
        ret = !obj->cache.data[0];
        obj->cache.data[0] = 1;
        return ret;
    }

    msiidx = htint_get_idx(obj->table, val);
    entry = obj->table.data[msiidx];
    if (entry.val) {
        return False;
    }

    vec_push_i32(&obj->cache, obj->arena, val);

    obj->table.data[msiidx].key = val;
    obj->table.data[msiidx].val = obj->cache.len - 1;
    ++obj->table.len;

    return True;
}

_fun b32 randomizedSetRemove(RandomizedSet* obj, i32 val) {
    i32 msiidx = 0;
    if (val == 0) {
        if (obj->cache.data[0]) {
            obj->cache.data[0] = 0;
            return True;
        }
        return False;
    }

    msiidx = htint_get_idx(obj->table, val);
    
    if (obj->table.data[msiidx].val) {
        i32 idxval = obj->table.data[msiidx].val;
        obj->table.data[msiidx].val = 0;

        idxval = idxval < obj->cache.len ? idxval : obj->cache.len - 1;
        while (obj->cache.data[idxval] != val) 
        {
            --idxval;
        }

        

        obj->cache.data[idxval] = obj->cache.data[--obj->cache.len];
        --obj->table.len;
        return True;
    }

    return False;
}

_fun i32 randomizedSetGetRandom(RandomizedSet* obj) {
    static u64 seed = 11111111111l;
    i32 range = obj->cache.data[0] ? obj->cache.len : obj->cache.len - 1;
    i32 choosen = (rnd(&seed) % range) + 1;

    if (choosen == obj->cache.len) {
        return False;
    }
    return obj->cache.data[choosen];
}

_proc randomizedSetFree(RandomizedSet* obj) {
    obj->arena->beg = 0;
    obj->arena->end = 0;
}

_proc printb32(i32 b) {
    if (b) {
        printf("true, ");
    }
    else {
        printf("false, ");
    }
}

_proc test(void) {
    RandomizedSet* obj = randomizedSetCreate();
    printf("[[], ");

    printb32(randomizedSetInsert(obj, 1));

    printb32(randomizedSetRemove(obj, 2));

    printb32(randomizedSetInsert(obj, 2));

    printf("%d, ", randomizedSetGetRandom(obj));

    printb32(randomizedSetRemove(obj, 1));

    printb32(randomizedSetInsert(obj, 2));
    
    printf("]");
    randomizedSetFree(obj);
    
}

i32 main(void) {
    test();
    return 0;
}
