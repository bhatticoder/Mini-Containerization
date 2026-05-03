#include "../include/Process.h"
#include <stdlib.h>
#include <string.h>

Process* Process_Create(int id, const char* name, int arrival, int burst, int priority) {
    Process* p = (Process*)malloc(sizeof(Process));
    if (!p) return NULL;
    
    p->pid = id;
    strncpy(p->name, name, sizeof(p->name) - 1);
    p->name[sizeof(p->name) - 1] = '\0';
    p->arrivalTime = arrival;
    p->burstTime = burst;
    p->remainingTime = burst;
    p->priority = priority;
    p->state = NEW;
    p->waitingTime = 0;
    p->turnaroundTime = 0;
    p->completionTime = 0;
    
    return p;
}

void Process_Destroy(Process* p) {
    if (p) free(p);
}
