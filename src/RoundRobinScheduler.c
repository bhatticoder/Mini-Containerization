#include "../include/RoundRobinScheduler.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define QUEUE_MAX 1024

int Process_CompareByArrival(const void* a, const void* b);

void RoundRobinScheduler_Schedule(Scheduler* self, Process* processes, int count) {
    if (!processes || count <= 0 || !self) return;
    
    RoundRobinScheduler* rr = (RoundRobinScheduler*)self;
    
    // Sort by arrival time
    qsort(processes, count, sizeof(Process), Process_CompareByArrival);
    
    int readyQueue[QUEUE_MAX];
    int queueFront = 0, queueSize = 0;
    int currentTime = 0;
    int completed = 0;
    int i = 0;
    
    while (completed < count) {
        // Add processes that have arrived
        while (i < count && processes[i].arrivalTime <= currentTime) {
            readyQueue[(queueFront + queueSize) % QUEUE_MAX] = i;
            queueSize++;
            i++;
        }
        
        if (queueSize == 0) {
            if (i < count) {
                currentTime = processes[i].arrivalTime;
            }
            continue;
        }
        
        int idx = readyQueue[queueFront];
        queueFront = (queueFront + 1) % QUEUE_MAX;
        queueSize--;
        
        int executeTime = (processes[idx].remainingTime < rr->timeQuantum) 
                          ? processes[idx].remainingTime 
                          : rr->timeQuantum;
        
        printf("Time %d: Running %s for %d units\n", currentTime, processes[idx].name, executeTime);
        
        processes[idx].remainingTime -= executeTime;
        currentTime += executeTime;
        
        // Add newly arrived processes
        while (i < count && processes[i].arrivalTime <= currentTime) {
            readyQueue[(queueFront + queueSize) % QUEUE_MAX] = i;
            queueSize++;
            i++;
        }
        
        if (processes[idx].remainingTime > 0) {
            readyQueue[(queueFront + queueSize) % QUEUE_MAX] = idx;
            queueSize++;
        } else {
            completed++;
            processes[idx].completionTime = currentTime;
            processes[idx].turnaroundTime = processes[idx].completionTime - processes[idx].arrivalTime;
            processes[idx].waitingTime = processes[idx].turnaroundTime - processes[idx].burstTime;
            processes[idx].state = TERMINATED;
        }
    }
    
    Scheduler_CalculateMetrics(processes, count);
}

void RoundRobinScheduler_Destroy(Scheduler* self) {
    if (self) free(self);
}

RoundRobinScheduler* RoundRobinScheduler_Create(int quantum) {
    RoundRobinScheduler* sched = (RoundRobinScheduler*)malloc(sizeof(RoundRobinScheduler));
    if (!sched) return NULL;
    
    sched->base.schedule = RoundRobinScheduler_Schedule;
    sched->base.destroy = RoundRobinScheduler_Destroy;
    sched->timeQuantum = quantum;
    
    return sched;
}
