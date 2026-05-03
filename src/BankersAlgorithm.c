#include "../include/BankersAlgorithm.h"
#include <string.h>

BankersAlgorithm* BankersAlgorithm_Create(int processes, int resources) {
    BankersAlgorithm* ba = (BankersAlgorithm*)malloc(sizeof(BankersAlgorithm));
    if (!ba) return NULL;
    
    ba->n = processes;
    ba->m = resources;
    
    // Initialize arrays to 0
    memset(ba->allocation, 0, sizeof(ba->allocation));
    memset(ba->max_need, 0, sizeof(ba->max_need));
    memset(ba->available, 0, sizeof(ba->available));
    
    return ba;
}

void BankersAlgorithm_Destroy(BankersAlgorithm* ba) {
    if (ba) free(ba);
}

void BankersAlgorithm_SetAvailable(BankersAlgorithm* ba, int* avail) {
    if (!ba || !avail) return;
    memcpy(ba->available, avail, ba->m * sizeof(int));
}

void BankersAlgorithm_SetAllocation(BankersAlgorithm* ba, int p, int* alloc) {
    if (!ba || !alloc || p >= ba->n) return;
    memcpy(ba->allocation[p], alloc, ba->m * sizeof(int));
}

void BankersAlgorithm_SetMaxNeed(BankersAlgorithm* ba, int p, int* max) {
    if (!ba || !max || p >= ba->n) return;
    memcpy(ba->max_need[p], max, ba->m * sizeof(int));
}

bool BankersAlgorithm_IsSafe(BankersAlgorithm* ba) {
    if (!ba) return false;
    
    int work[MAX_RESOURCES];
    bool finish[MAX_PROCESSES];
    int need[MAX_PROCESSES][MAX_RESOURCES];
    
    // Initialize work and finish
    memcpy(work, ba->available, ba->m * sizeof(int));
    memset(finish, 0, sizeof(finish));
    
    // Calculate need
    for (int i = 0; i < ba->n; i++) {
        for (int j = 0; j < ba->m; j++) {
            need[i][j] = ba->max_need[i][j] - ba->allocation[i][j];
        }
    }
    
    int count = 0;
    while (count < ba->n) {
        bool found = false;
        
        for (int p = 0; p < ba->n; p++) {
            if (!finish[p]) {
                int j;
                // Check if need[p] <= work
                for (j = 0; j < ba->m; j++) {
                    if (need[p][j] > work[j]) {
                        break;
                    }
                }
                
                if (j == ba->m) {
                    // Process p can complete
                    for (int k = 0; k < ba->m; k++) {
                        work[k] += ba->allocation[p][k];
                    }
                    finish[p] = true;
                    found = true;
                    count++;
                }
            }
        }
        
        if (!found) {
            return false;  // Unsafe state
        }
    }
    
    return true;  // Safe state
}
