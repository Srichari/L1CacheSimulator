#ifndef BUS_HPP
#define BUS_HPP

#include <vector>
#include <string>
#include "Cache.hpp"

struct BusRequest {
    int coreId;
    std::string type; // READ, WRITE, INVALIDATE
    uint32_t addr;
};

class Cache; // forward declaration

class Bus {
public:
    void addCache(Cache* cache);
    void broadcast(const BusRequest& request);
    void snoop(Cache* source, const BusRequest& request);
    int getInvalidations() const;
    int getTraffic() const;

private:
    std::vector<Cache*> caches;
    int invalidations = 0;
    int traffic = 0;
};

#endif // BUS_HPP
