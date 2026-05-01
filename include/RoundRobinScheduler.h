#ifndef ROUND_ROBIN_SCHEDULER_H
#define ROUND_ROBIN_SCHEDULER_H

#include "Scheduler.h"
#include <queue>
#include <algorithm>

class RoundRobinScheduler : public Scheduler {
    int timeQuantum;
public:
    RoundRobinScheduler(int quantum) : timeQuantum(quantum) {}

    void schedule(std::vector<Process>& processes) override {
        std::queue<int> readyQueue;
        int currentTime = 0;
        int completed = 0;
        int n = processes.size();

        std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
            return a.arrivalTime < b.arrivalTime;
        });

        int i = 0;
        while (completed < n) {
            while (i < n && processes[i].arrivalTime <= currentTime) {
                readyQueue.push(i);
                i++;
            }

            if (readyQueue.empty()) {
                currentTime = processes[i].arrivalTime;
                continue;
            }

            int idx = readyQueue.front();
            readyQueue.pop();

            int executeTime = std::min(processes[idx].remainingTime, timeQuantum);
            std::cout << "Time " << currentTime << ": Running " << processes[idx].name << " for " << executeTime << " units" << std::endl;
            
            processes[idx].remainingTime -= executeTime;
            currentTime += executeTime;

            while (i < n && processes[i].arrivalTime <= currentTime) {
                readyQueue.push(i);
                i++;
            }

            if (processes[idx].remainingTime > 0) {
                readyQueue.push(idx);
            } else {
                completed++;
                processes[idx].completionTime = currentTime;
                processes[idx].turnaroundTime = processes[idx].completionTime - processes[idx].arrivalTime;
                processes[idx].waitingTime = processes[idx].turnaroundTime - processes[idx].burstTime;
                processes[idx].state = ProcessState::TERMINATED;
            }
        }
        calculateMetrics(processes);
    }
};

#endif
