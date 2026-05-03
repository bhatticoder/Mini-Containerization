#include "../include/FIFOMemoryManager.h"

FIFOMemoryManager* FIFOMemoryManager_Create(int capacity) {
    FIFOMemoryManager* fm = (FIFOMemoryManager*)malloc(sizeof(FIFOMemoryManager));
    if (!fm) return NULL;
    
    fm->capacity = capacity;
    fm->page_faults = 0;
    fm->queue_front = 0;
    fm->queue_size = 0;
    
    for (int i = 0; i < MAX_PAGES; i++) {
        fm->page_set[i] = 0;
    }
    
    return fm;
}

void FIFOMemoryManager_Destroy(FIFOMemoryManager* fm) {
    if (fm) free(fm);
}

void FIFOMemoryManager_AccessPage(FIFOMemoryManager* fm, int pageId) {
    if (!fm || pageId >= MAX_PAGES) return;
    
    if (fm->page_set[pageId] == 0) {
        // Page fault
        fm->page_faults++;
        
        if (fm->queue_size >= fm->capacity) {
            // Remove oldest page (front of queue)
            int oldest = fm->fifo_queue[fm->queue_front];
            fm->queue_front = (fm->queue_front + 1) % MAX_PAGES;
            fm->queue_size--;
            fm->page_set[oldest] = 0;
        }
        
        // Add new page to queue
        int rear = (fm->queue_front + fm->queue_size) % MAX_PAGES;
        fm->fifo_queue[rear] = pageId;
        fm->queue_size++;
        fm->page_set[pageId] = 1;
    }
}

int FIFOMemoryManager_GetPageFaults(FIFOMemoryManager* fm) {
    return fm ? fm->page_faults : 0;
}
