#ifndef FIFO_MEMORY_MANAGER_H
#define FIFO_MEMORY_MANAGER_H

#include <vector>
#include <queue>
#include <unordered_set>
#include <iostream>

class FIFOMemoryManager {
    int capacity;
    std::queue<int> fifo_queue;
    std::unordered_set<int> page_set;
    int page_faults;

public:
    FIFOMemoryManager(int cap) : capacity(cap), page_faults(0) {}

    void accessPage(int pageId) {
        if (page_set.find(pageId) == page_set.end()) {
            page_faults++;
            if (fifo_queue.size() == capacity) {
                int oldest = fifo_queue.front();
                fifo_queue.pop();
                page_set.erase(oldest);
            }
            fifo_queue.push(pageId);
            page_set.insert(pageId);
        }
    }

    int getPageFaults() { return page_faults; }
};

#endif
