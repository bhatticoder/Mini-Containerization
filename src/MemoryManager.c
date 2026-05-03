#include "../include/MemoryManager.h"
#include <stdlib.h>

MemoryManager* MemoryManager_Create(int capacity) {
    MemoryManager* mm = (MemoryManager*)malloc(sizeof(MemoryManager));
    if (!mm) return NULL;
    
    mm->capacity = capacity;
    mm->page_faults = 0;
    mm->head = NULL;
    mm->page_map_size = 1024;
    mm->page_map = (int*)calloc(mm->page_map_size, sizeof(int));
    
    if (!mm->page_map) {
        free(mm);
        return NULL;
    }
    
    return mm;
}

void MemoryManager_Destroy(MemoryManager* mm) {
    if (!mm) return;
    
    Node* current = mm->head;
    while (current) {
        Node* tmp = current;
        current = current->next;
        free(tmp);
    }
    
    if (mm->page_map) free(mm->page_map);
    free(mm);
}

void MemoryManager_AccessPage(MemoryManager* mm, int pageId) {
    if (!mm) return;
    
    // Ensure page_map is big enough
    if (pageId >= mm->page_map_size) {
        int newSize = pageId + 100;
        int* newMap = (int*)realloc(mm->page_map, newSize * sizeof(int));
        if (!newMap) return;
        mm->page_map = newMap;
        for (int i = mm->page_map_size; i < newSize; i++) {
            mm->page_map[i] = 0;
        }
        mm->page_map_size = newSize;
    }
    
    if (mm->page_map[pageId] == 0) {
        // Page fault
        mm->page_faults++;
        
        // Count current pages in list
        int count = 0;
        Node* tmp = mm->head;
        while (tmp) {
            count++;
            tmp = tmp->next;
        }
        
        if (count >= mm->capacity) {
            // Remove last (least recently used)
            if (mm->head->next == NULL) {
                int oldPageId = mm->head->pageId;
                free(mm->head);
                mm->head = NULL;
                mm->page_map[oldPageId] = 0;
            } else {
                Node* current = mm->head;
                while (current->next->next) {
                    current = current->next;
                }
                int oldPageId = current->next->pageId;
                free(current->next);
                current->next = NULL;
                mm->page_map[oldPageId] = 0;
            }
        }
    } else {
        // Page hit - move to front
        if (mm->head && mm->head->pageId != pageId) {
            Node* current = mm->head;
            while (current->next && current->next->pageId != pageId) {
                current = current->next;
            }
            if (current->next) {
                Node* tmp = current->next;
                current->next = tmp->next;
                tmp->next = mm->head;
                mm->head = tmp;
            }
            return;
        }
    }
    
    // Add to front
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) return;
    newNode->pageId = pageId;
    newNode->next = mm->head;
    newNode->prev = NULL;
    if (mm->head) mm->head->prev = newNode;
    mm->head = newNode;
    mm->page_map[pageId] = 1;
}

int MemoryManager_GetPageFaults(MemoryManager* mm) {
    return mm ? mm->page_faults : 0;
}
