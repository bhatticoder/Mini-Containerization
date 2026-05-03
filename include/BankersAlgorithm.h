#ifndef BANKERS_ALGORITHM_H
#define BANKERS_ALGORITHM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_PROCESSES 100
#define MAX_RESOURCES 100

typedef struct {
    int n;  // number of processes
    int m;  // number of resources
    int allocation[MAX_PROCESSES][MAX_RESOURCES];
    int max_need[MAX_PROCESSES][MAX_RESOURCES];
    int available[MAX_RESOURCES];
} BankersAlgorithm;

BankersAlgorithm* BankersAlgorithm_Create(int processes, int resources);
void BankersAlgorithm_Destroy(BankersAlgorithm* ba);
void BankersAlgorithm_SetAvailable(BankersAlgorithm* ba, int* avail);
void BankersAlgorithm_SetAllocation(BankersAlgorithm* ba, int p, int* alloc);
void BankersAlgorithm_SetMaxNeed(BankersAlgorithm* ba, int p, int* max);
bool BankersAlgorithm_IsSafe(BankersAlgorithm* ba);

#endif
