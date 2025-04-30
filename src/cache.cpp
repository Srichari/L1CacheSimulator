#include "Cache.hpp"
#include <cmath>
#include <iostream>

Cache::Cache(int s, int E, int b, int coreId, Bus* bus)
    : s(s), E(E), b(b), coreId(coreId), bus(bus) {
    S = 1 << s;
    B = 1 << b;
    sets.reserve(S);
    for (int i = 0; i < S; ++i) sets.emplace_back(E);
}

bool Cache::access(bool isWrite, unsigned long address) {
    unsigned long index = (address >> b) & ((1 << s) - 1);
    unsigned long tag = address >> (s + b);
    CacheSet& set = sets[index];
    int hitIndex = set.findLine(tag);

    if (isWrite) writeCount++;
    else readCount++;

    if (hitIndex != -1) {
        CacheLine& line = set.lines[hitIndex];
        line.lruCounter = 0;
        line.state = isWrite ? M : line.state;
        return true;
    }

    missCount++;
    int victimIndex = set.findLRUVictim();
    CacheLine& victim = set.lines[victimIndex];

    if (victim.valid && victim.dirty) {
        writebacks++;
    }

    victim.valid = true;
    victim.tag = tag;
    victim.dirty = isWrite;
    victim.state = isWrite ? M : E;
    victim.lruCounter = 0;

    return false;
}

void Cache::snoop(const BusRequest& req) {
    // Implement MESI transitions here.
}

void Cache::printStats() {
    std::cout << "Core " << coreId << "\n";
    std::cout << "Reads: " << readCount << ", Writes: " << writeCount << "\n";
    std::cout << "Misses: " << missCount << ", Evictions: " << evictions << ", Writebacks: " << writebacks << "\n";
}

