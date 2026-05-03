#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int pageId;
    struct Node* prev;
    struct Node* next;
} Node;

typedef struct {
    int capacity;
    int page_faults;
    Node* head;
    int* page_map;           // Hash map: pageId -> 1 if present, 0 otherwise
    int page_map_size;
} MemoryManager;

MemoryManager* MemoryManager_Create(int capacity);
void MemoryManager_Destroy(MemoryManager* mm);
void MemoryManager_AccessPage(MemoryManager* mm, int pageId);
int MemoryManager_GetPageFaults(MemoryManager* mm);

#endif
