#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Process.h"
#include <vector>

class Scheduler {
public:
    virtual void schedule(std::vector<Process>& processes) = 0;
    virtual ~Scheduler() {}

    void calculateMetrics(std::vector<Process>& processes) {
        float avgWaiting = 0;
        float avgTurnaround = 0;
        for (const auto& p : processes) {
            avgWaiting += p.waitingTime;
            avgTurnaround += p.turnaroundTime;
        }
        std::cout << "Average Waiting Time: " << avgWaiting / processes.size() << std::endl;
        std::cout << "Average Turnaround Time: " << avgTurnaround / processes.size() << std::endl;
    }
};

#endif
