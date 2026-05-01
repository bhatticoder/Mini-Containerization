#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>
#include <vector>
#include <string>

enum class ProcessState {
    NEW,
    READY,
    RUNNING,
    WAITING,
    TERMINATED
};

class Process {
public:
    int pid;
    std::string name;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int priority;
    ProcessState state;
    int waitingTime;
    int turnaroundTime;
    int completionTime;

    Process(int id, std::string n, int arrival, int burst, int prio = 0)
        : pid(id), name(n), arrivalTime(arrival), burstTime(burst), 
          remainingTime(burst), priority(prio), state(ProcessState::NEW),
          waitingTime(0), turnaroundTime(0), completionTime(0) {}
};

#endif
