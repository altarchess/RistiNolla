#include "tt.h"

/***
 * init tt
 ***/

void TT::init() {
    entries = std::make_unique<Entry[]>(TT_MASK + 1);
    age = 0;
}

/***
 * get entry from tt
 ***/

Entry TT::get(uint64_t key) {
    return entries[key & TT_MASK];
}

/***
 * put entry into tt
 ***/

void TT::put(uint64_t k, uint8_t t, uint16_t d, int16_t s, int16_t m) {
    entries[k & TT_MASK].set(k, age, t, d, s, m);
}

/***
 * Age is used for replacing of old high depth entries, increment age each time we have a new search.
 ***/

void TT::incrementAge() {
    age++;
}

/***
 * try to prefetch tt entry
 ***/

void TT::prefetch(uint64_t key) {
    __builtin_prefetch(&entries[key & TT_MASK]);
}