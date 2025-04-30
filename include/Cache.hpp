#ifndef CACHE_HPP
#define CACHE_HPP

#include "CacheSet.h"
#include "Bus.hpp"
#include <vector>

class Bus;

class Cache {
public:
    int s, E, b, coreId;
    int S, B;
    std::vector<CacheSet> sets;
    Bus* bus;

    int readCount = 0;
    int writeCount = 0;
    int missCount = 0;
    int evictions = 0;
    int writebacks = 0;

    Cache(int s, int E, int b, int coreId, Bus* bus);
    bool access(bool isWrite, unsigned long address);
    void snoop(const BusRequest& req);
    void printStats();
};

#endif
