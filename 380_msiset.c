#include <stdio.h>
#include "ale.h"

typedef struct {
    Ht64 table;
    Vec64 cache;
    Arena * arena;
} RandomizedSet;


_proc print_cache(RandomizedSet* obj) {
    printf("|Cache . .");
    for(i32 i = 0; i < obj->cache.len; ++i) {
        printf(" %d:%lld ", i, obj->cache.data[i]);
    }
    printf("| ");
}

_fun RandomizedSet * randomizedSetCreate(void) {
    static u8 memory[8*MBs_] = {0};
    static Arena arena = {0, 0};

    arena = newarena(sizeof(memory), memory);
    
    {
        RandomizedSet *S = (RandomizedSet *) alloc(&arena, sizeof(RandomizedSet), 1);
        S->arena = &arena;

        S->table = new_Ht64(&arena, 200000);
        vec_push_i64(&S->cache, &arena, 0);
        return S;
    }
}

_fun b32 randomizedSetInsert(RandomizedSet* obj, i32 val) {
    i32 msiidx = 0;
    Entry64 entry = {0, 0};
    i64 key = val;

    b32 ret = False;
    if (key == 0) {
        ret = !obj->cache.data[0];
        obj->cache.data[0] = 1;
        return ret;
    }

    msiidx = hti_get_idx(obj->table, key);
    entry = obj->table.data[msiidx];
    if (entry.val) {
        return False;
    }

    vec_push_i64(&obj->cache, obj->arena, key);

    obj->table.data[msiidx].key = key;
    obj->table.data[msiidx].val = obj->cache.len - 1;
    ++obj->table.len;

    return True;
}

_fun b32 randomizedSetRemove(RandomizedSet* obj, i32 element) {
    i32 msiidx = 0;
    i64 key = element;

    if (key == 0) {
        if (obj->cache.data[0]) {
            obj->cache.data[0] = 0;
            return True;
        }
        return False;
    }

    msiidx = hti_get_idx(obj->table, key);
    
    if (obj->table.data[msiidx].val) {
        i64 idxelement = obj->table.data[msiidx].val;
        obj->table.data[msiidx].val = 0;

        idxelement = idxelement < obj->cache.len ? idxelement : obj->cache.len - 1;
        while (obj->cache.data[idxelement] != key) 
        {
            --idxelement;
        }

        

        obj->cache.data[idxelement] = obj->cache.data[--obj->cache.len];
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
    return (i32) obj->cache.data[choosen];
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
