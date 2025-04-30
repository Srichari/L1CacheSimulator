#include "CacheLine.hpp"

CacheLine::CacheLine() {
    valid = false;
    dirty = false;
    tag = 0;
    state = INVALID;
    lastUsedCycle = 0;
}

void CacheLine::invalidate() {
    valid = false;
    dirty = false;
    state = INVALID;
}

