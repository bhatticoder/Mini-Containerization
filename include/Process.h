#ifndef PROCESS_H
#define PROCESS_H

#include <string.h>
#include <stdlib.h>

typedef enum {
    NEW,
    READY,
    RUNNING,
    WAITING,
    TERMINATED
} ProcessState;

typedef struct {
    int pid;
    char name[256];
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int priority;
    ProcessState state;
    int waitingTime;
    int turnaroundTime;
    int completionTime;
} Process;

// Constructor-like function
Process* Process_Create(int id, const char* name, int arrival, int burst, int priority);

// Destructor-like function
void Process_Destroy(Process* p);

#endif
