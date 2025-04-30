#include "Cache.hpp"
#include "Bus.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <sstream>
#include <iomanip>
#include <getopt.h>

struct TraceEntry {
    char op;
    uint32_t address;
};

int main(int argc, char** argv) {
    std::string tracePrefix, outFilename;
    int s = 5, E = 2, b = 5;
    
    // Parse command-line arguments
    int opt;
    while ((opt = getopt(argc, argv, "t:s:E:b:o:h")) != -1) {
        switch (opt) {
            case 't': tracePrefix = optarg; break;
            case 's': s = atoi(optarg); break;
            case 'E': E = atoi(optarg); break;
            case 'b': b = atoi(optarg); break;
            case 'o': outFilename = optarg; break;
            case 'h':
                std::cout << "./L1simulate -t <trace_prefix> -s <s> -E <E> -b <b> -o <outfile>\n";
                return 0;
        }
    }

    // Open output file
    std::ofstream ofs(outFilename);
    if (!ofs) {
        std::cerr << "Could not open output file.\n";
        return 1;
    }

    const int NUM_CORES = 4;
    std::vector<std::ifstream> traceFiles(NUM_CORES);
    std::vector<std::queue<TraceEntry>> traces(NUM_CORES);
    std::vector<bool> done(NUM_CORES, false);
    std::vector<int> cycles(NUM_CORES, 0);
    std::vector<int> idleCycles(NUM_CORES, 0);

    // Load trace files
    for (int i = 0; i < NUM_CORES; ++i) {
        std::string filename = tracePrefix + "_proc" + std::to_string(i) + ".trace";
        traceFiles[i].open(filename);
        if (!traceFiles[i]) {
            std::cerr << "Error opening " << filename << "\n";
            return 1;
        }
    }

    // Shared bus and cache instances
    Bus bus;
    std::vector<Cache*> caches;

    for (int i = 0; i < NUM_CORES; ++i) {
        Cache* cache = new Cache(s, E, b, i, &bus);
        caches.push_back(cache);
        bus.addCache(cache);
    }

    // Preload one instruction per core
    for (int i = 0; i < NUM_CORES; ++i) {
        std::string line;
        if (std::getline(traceFiles[i], line)) {
            std::istringstream iss(line);
            char op;
            std::string addrStr;
            iss >> op >> addrStr;
            uint32_t addr = std::stoul(addrStr, nullptr, 16);
            traces[i].push({op, addr});
        } else {
            done[i] = true;
        }
    }

    // Simulate cycle by cycle
    bool allDone = false;
    while (!allDone) {
        allDone = true;
        for (int i = 0; i < NUM_CORES; ++i) {
            if (done[i]) continue;

            allDone = false;
            if (traces[i].empty()) continue;

            TraceEntry entry = traces[i].front();
            bool wasHit = caches[i]->access(entry.op, entry.address);

            if (!wasHit) idleCycles[i] += 100; // Memory latency
            else cycles[i] += 1;

            traces[i].pop();

            // Read next instruction
            std::string line;
            if (std::getline(traceFiles[i], line)) {
                std::istringstream iss(line);
                char op;
                std::string addrStr;
                iss >> op >> addrStr;
                uint32_t addr = std::stoul(addrStr, nullptr, 16);
                traces[i].push({op, addr});
            } else {
                done[i] = true;
            }
        }
    }

    // Output statistics
    for (int i = 0; i < NUM_CORES; ++i) {
        ofs << "Core " << i << ":\n";
        caches[i]->printStats();
        int total = caches[i]->getReadCount() + caches[i]->getWriteCount();
        float missRate = static_cast<float>(caches[i]->getMissCount()) / total;
        ofs << "Execution cycles: " << cycles[i] + idleCycles[i] << "\n";
        ofs << "Idle cycles: " << idleCycles[i] << "\n";
        ofs << "Miss rate: " << std::fixed << std::setprecision(4) << missRate << "\n";
        ofs << "Evictions: " << caches[i]->getEvictionCount() << "\n";
        ofs << "Writebacks: " << caches[i]->getWritebackCount() << "\n\n";
    }

    // Cleanup
    for (auto* c : caches) delete c;
    for (auto& f : traceFiles) f.close();

    return 0;
}
