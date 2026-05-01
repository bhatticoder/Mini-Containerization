#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <vector>
#include <unordered_map>
#include <list>
#include <iostream>

class MemoryManager {
    int capacity;
    std::list<int> lru_list;
    std::unordered_map<int, std::list<int>::iterator> page_map;
    int page_faults;

public:
    MemoryManager(int cap) : capacity(cap), page_faults(0) {}

    void accessPage(int pageId) {
        if (page_map.find(pageId) == page_map.end()) {
            // Page fault
            page_faults++;
            if (lru_list.size() == capacity) {
                int last = lru_list.back();
                lru_list.pop_back();
                page_map.erase(last);
            }
        } else {
            // Page hit - move to front
            lru_list.erase(page_map[pageId]);
        }
        lru_list.push_front(pageId);
        page_map[pageId] = lru_list.begin();
    }

    int getPageFaults() { return page_faults; }
};

#endif
