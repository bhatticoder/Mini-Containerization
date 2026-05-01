#ifndef FCFS_SCHEDULER_H
#define FCFS_SCHEDULER_H

#include "Scheduler.h"
#include <algorithm>

class FCFSScheduler : public Scheduler {
public:
    void schedule(std::vector<Process>& processes) override {
        std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
            return a.arrivalTime < b.arrivalTime;
        });

        int currentTime = 0;
        for (auto& p : processes) {
            if (currentTime < p.arrivalTime) {
                currentTime = p.arrivalTime;
            }
            std::cout << "Time " << currentTime << ": Starting " << p.name << " (Burst: " << p.burstTime << ")" << std::endl;
            p.waitingTime = currentTime - p.arrivalTime;
            currentTime += p.burstTime;
            p.completionTime = currentTime;
            p.turnaroundTime = p.completionTime - p.arrivalTime;
            p.state = ProcessState::TERMINATED;
        }
        calculateMetrics(processes);
    }
};

#endif
