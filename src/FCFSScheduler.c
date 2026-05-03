#include "../include/FCFSScheduler.h"
#include <stdlib.h>
#include <stdio.h>

// Comparison function for qsort
int Process_CompareByArrival(const void* a, const void* b) {
    const Process* p1 = (const Process*)a;
    const Process* p2 = (const Process*)b;
    return p1->arrivalTime - p2->arrivalTime;
}

void FCFSScheduler_Schedule(Scheduler* self, Process* processes, int count) {
    if (!processes || count <= 0) return;
    
    // Sort by arrival time
    qsort(processes, count, sizeof(Process), Process_CompareByArrival);
    
    int currentTime = 0;
    for (int i = 0; i < count; i++) {
        if (currentTime < processes[i].arrivalTime) {
            currentTime = processes[i].arrivalTime;
        }
        printf("Time %d: Starting %s (Burst: %d)\n", currentTime, processes[i].name, processes[i].burstTime);
        
        processes[i].waitingTime = currentTime - processes[i].arrivalTime;
        currentTime += processes[i].burstTime;
        processes[i].completionTime = currentTime;
        processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;
        processes[i].state = TERMINATED;
    }
    
    Scheduler_CalculateMetrics(processes, count);
}

void FCFSScheduler_Destroy(Scheduler* self) {
    if (self) free(self);
}

FCFSScheduler* FCFSScheduler_Create(void) {
    FCFSScheduler* sched = (FCFSScheduler*)malloc(sizeof(FCFSScheduler));
    if (!sched) return NULL;
    
    sched->base.schedule = FCFSScheduler_Schedule;
    sched->base.destroy = FCFSScheduler_Destroy;
    
    return sched;
}
