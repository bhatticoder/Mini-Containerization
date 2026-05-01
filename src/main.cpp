#include "Process.h"
#include "RoundRobinScheduler.h"
#include "ContainerManager.h"
#include "ResourceSync.h"
#include "IPCManager.h"
#include "BankersAlgorithm.h"
#include "MemoryManager.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "Process.h"
#include "RoundRobinScheduler.h"
#include "FCFSScheduler.h"
#include "ContainerManager.h"
#include "ResourceSync.h"
#include "IPCManager.h"
#include "BankersAlgorithm.h"
#include "MemoryManager.h"
#include "FIFOMemoryManager.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

void printHelp() {
    std::cout << "\n==============================================" << std::endl;
    std::cout << "    LITECONTAINER ENGINE - DOCKER CLI" << std::endl;
    std::cout << "==============================================" << std::endl;
    std::cout << "COMMANDS:" << std::endl;
    std::cout << "  config <ram_mb> <disk_mb>           - Set total system resources" << std::endl;
    std::cout << "  run <name> <task> [-m mem] [-d disk] - Create container with quotas" << std::endl;
    std::cout << "  ps                                  - List containers & usage" << std::endl;
    std::cout << "  stop <name>                         - Stop container" << std::endl;
    std::cout << "  stats/compare                       - Benchmarking & Simulation Reports" << std::endl;
    std::cout << "  sync                                - Resource sync demo" << std::endl;
    std::cout << "  help                                - Show help" << std::endl;
    std::cout << "  exit                                - Exit" << std::endl;
    std::cout << "==============================================\n" << std::endl;
}

int main() {
    ContainerManager cm;
    printHelp();

    std::string line;
    while (true) {
        std::cout << "docker-lite> ";
        if (!std::getline(std::cin, line) || line == "exit") break;

        std::stringstream ss(line);
        std::string cmd;
        ss >> cmd;

        if (cmd == "config") {
            int r, d;
            if (ss >> r >> d) cm.setGlobalResources(r, d);
            else std::cout << "Usage: config <ram_mb> <disk_mb>" << std::endl;
        }
        else if (cmd == "run") {
            std::string name, task, part;
            int mem = 256, disk = 1000, prio = 1;
            
            if (!(ss >> name)) {
                std::cout << "Error: run requires an image/name." << std::endl;
                continue;
            }

            // Read task until we hit a flag or end of line
            std::string fullTask;
            while (ss >> part) {
                if (part[0] == '-') {
                    // It's a flag, handle it and break task collection
                    if (part == "-m") ss >> mem;
                    else if (part == "-d") ss >> disk;
                    else if (part == "-p") ss >> prio;
                    
                    // Continue to check for other flags
                    while (ss >> part) {
                        if (part == "-m") ss >> mem;
                        else if (part == "-d") ss >> disk;
                        else if (part == "-p") ss >> prio;
                    }
                    break;
                } else {
                    if (!fullTask.empty()) fullTask += " ";
                    fullTask += part;
                }
            }
            
            if (fullTask.empty()) fullTask = "default-task";
            cm.createContainer(name, fullTask, mem, disk, prio);
        } 
        else if (cmd == "ps") {
            cm.listContainers();
        } 
        else if (cmd == "stop") {
            std::string name;
            if (ss >> name) cm.stopContainer(name);
            else std::cout << "Usage: stop <name>" << std::endl;
        }
        else if (cmd == "clear") {
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            printHelp();
        }
        else if (cmd == "stats" || cmd == "compare" || cmd == "state") {
            std::cout << "\n[Module 2/6] ALGORITHM COMPARISON REPORT" << std::endl;
            
            // Scheduling comparison
            std::cout << "\n--- CPU Scheduling Comparison ---" << std::endl;
            std::vector<Process> pbatch = { Process(1, "Job1", 0, 10), Process(2, "Job2", 2, 5) };
            std::cout << ">> FCFS Strategy:" << std::endl;
            FCFSScheduler fcfs; fcfs.schedule(pbatch);
            
            pbatch = { Process(1, "Job1", 0, 10), Process(2, "Job2", 2, 5) };
            std::cout << "\n>> Round Robin (Quantum=3) Strategy:" << std::endl;
            RoundRobinScheduler rr(3); rr.schedule(pbatch);

            // Memory comparison
            std::cout << "\n--- Memory Page Replacement Comparison ---" << std::endl;
            std::vector<int> stream = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
            MemoryManager lru(3);
            FIFOMemoryManager fifo(3);
            for(int p : stream) { lru.accessPage(p); fifo.accessPage(p); }
            std::cout << "LRU Page Faults: " << lru.getPageFaults() << std::endl;
            std::cout << "FIFO Page Faults: " << fifo.getPageFaults() << std::endl;
        }
        else if (cmd == "sync") {
            ResourceSync sync; sync.accessSharedFile(1); sync.useSharedDevice(2);
        }
        else if (cmd == "help") {
            printHelp();
        }
    }
    return 0;
}
