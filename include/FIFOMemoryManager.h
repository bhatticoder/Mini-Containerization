#ifndef FIFO_MEMORY_MANAGER_H
#define FIFO_MEMORY_MANAGER_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_PAGES 1024

typedef struct {
    int capacity;
    int page_faults;
    int fifo_queue[MAX_PAGES];
    int queue_front;
    int queue_size;
    int page_set[MAX_PAGES];  // 1 if page is present, 0 otherwise
} FIFOMemoryManager;

FIFOMemoryManager* FIFOMemoryManager_Create(int capacity);
void FIFOMemoryManager_Destroy(FIFOMemoryManager* fm);
void FIFOMemoryManager_AccessPage(FIFOMemoryManager* fm, int pageId);
int FIFOMemoryManager_GetPageFaults(FIFOMemoryManager* fm);

#endif
