#include "../include/Scheduler.h"
#include <stdio.h>

void Scheduler_CalculateMetrics(Process* processes, int count) {
    if (!processes || count <= 0) return;
    
    float avgWaiting = 0;
    float avgTurnaround = 0;
    
    for (int i = 0; i < count; i++) {
        avgWaiting += processes[i].waitingTime;
        avgTurnaround += processes[i].turnaroundTime;
    }
    
    printf("Average Waiting Time: %.2f\n", avgWaiting / count);
    printf("Average Turnaround Time: %.2f\n", avgTurnaround / count);
}
