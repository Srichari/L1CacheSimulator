
#ifndef CACHESET_HPP
#define CACHESET_HPP

#include "CacheLine.hpp"
#include <vector>

class CacheSet {
public:
    std::vector<CacheLine> lines;
    int E;

    CacheSet(int E);
    int findLine(uint32_t tag);
    int getLRUVictim();
};

#endif // CACHESET_HPP
