
#ifndef CACHELINE_HPP
#define CACHELINE_HPP

#include <cstdint>

enum MESIState {
    INVALID,
    SHARED,
    EXCLUSIVE,
    MODIFIED
};

class CacheLine {
public:
    bool valid;
    bool dirty;
    uint32_t tag;
    MESIState state;
    uint64_t lastUsedCycle;

    CacheLine();
    void invalidate();
};

#endif // CACHELINE_HPP
