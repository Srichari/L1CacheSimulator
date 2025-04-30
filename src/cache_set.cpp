
#include "CacheSet.hpp"
#include <limits>

CacheSet::CacheSet(int E) : E(E), lines(E) {}

int CacheSet::findLine(uint32_t tag) {
    for (int i = 0; i < E; ++i) {
        if (lines[i].valid && lines[i].tag == tag) return i;
    }
    return -1;
}

int CacheSet::getLRUVictim() {
    uint64_t minCycle = std::numeric_limits<uint64_t>::max();
    int victim = 0;
    for (int i = 0; i < E; ++i) {
        if (!lines[i].valid) return i;
        if (lines[i].lastUsedCycle < minCycle) {
            minCycle = lines[i].lastUsedCycle;
            victim = i;
        }
    }
    return victim;
}
