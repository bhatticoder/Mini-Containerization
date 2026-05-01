#ifndef CONTAINER_MANAGER_H
#define CONTAINER_MANAGER_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include "Process.h"

namespace fs = std::filesystem;

#ifdef _WIN32
#include <windows.h>
typedef int pid_t;
#else
#include <unistd.h>
#include <sys/wait.h>
#endif

struct ContainerInfo {
    std::string name;
    std::string status;
    int pid;
    int memoryLimit;
    int cpuPriority;
    int diskLimit;
};

class ContainerManager {
    std::vector<ContainerInfo> activeContainers;
    const std::string STATE_FILE = "containers/engine_state.dat";
    int totalRAM = 8192;  // Default 8GB
    int totalDisk = 50000; // Default 50GB
    int usedRAM = 0;
    int usedDisk = 0;

public:
    ContainerManager() {
        loadState();
    }

    void setGlobalResources(int ram, int disk) {
        totalRAM = ram;
        totalDisk = disk;
        std::cout << "[Host] System Resources Set: RAM=" << totalRAM << "MB, Disk=" << totalDisk << "MB" << std::endl;
    }

    void createContainer(const std::string& containerName, const std::string& taskData, int memLimit = 100, int diskLimit = 500, int priority = 1) {
        // Resource Validation
        if (usedRAM + memLimit > totalRAM) {
            std::cerr << "Error: Not enough RAM! Required: " << memLimit << "MB, Available: " << (totalRAM - usedRAM) << "MB" << std::endl;
            return;
        }
        if (usedDisk + diskLimit > totalDisk) {
            std::cerr << "Error: Not enough Disk! Required: " << diskLimit << "MB, Available: " << (totalDisk - usedDisk) << "MB" << std::endl;
            return;
        }

        std::string workspace = "containers/" + containerName;
        try {
            if (!fs::exists(workspace)) {
                fs::create_directories(workspace);
            }
        } catch (const std::exception& e) {
            std::cerr << "Error creating workspace: " << e.what() << std::endl;
            return;
        }

        // Store Task info in file
        std::string taskFile = workspace + "/task.json";
        std::ofstream outfile(taskFile);
        outfile << "{" << "\n  \"name\": \"" << containerName << "\","
                << "\n  \"task\": \"" << taskData << "\","
                << "\n  \"mem\": " << memLimit << ","
                << "\n  \"disk\": " << diskLimit << ","
                << "\n  \"priority\": " << priority << "\n}";
        outfile.close();

#ifdef _WIN32
        activeContainers.push_back({containerName, "Running", (int)activeContainers.size() + 100, memLimit, priority, diskLimit});
        usedRAM += memLimit;
        usedDisk += diskLimit;
        saveState();
        std::cout << "[Host] Container " << containerName << " successfully deployed." << std::endl;
#else
        pid_t pid = fork();
        if (pid == 0) {
            fs::current_path(workspace);
            while(true) { sleep(10); } 
            exit(0);
        } else {
            activeContainers.push_back({containerName, "Running", pid, memLimit, priority, diskLimit});
            usedRAM += memLimit;
            usedDisk += diskLimit;
            saveState();
        }
#endif
    }

    void listContainers() {
        std::cout << "\nSYSTEM LOAD: [RAM: " << usedRAM << "/" << totalRAM << "MB] [DISK: " << usedDisk << "/" << totalDisk << "MB]" << std::endl;
        std::cout << "--------------------------------------------------------------------------------" << std::endl;
        std::cout << "CONTAINER ID\tNAME\t\tSTATUS\t\tMEM\tDISK\tPRIO" << std::endl;
        std::cout << "--------------------------------------------------------------------------------" << std::endl;
        for (const auto& c : activeContainers) {
            std::cout << c.pid << "\t\t" << c.name << "\t\t" << c.status << "\t\t" << c.memoryLimit << "M\t" << c.diskLimit << "M\t" << c.cpuPriority << std::endl;
        }
    }

    void stopContainer(const std::string& name) {
        for (auto& c : activeContainers) {
            if (c.name == name) {
                if (c.status == "Exited") {
                    std::cout << "Container " << name << " is already stopped." << std::endl;
                    return;
                }
                c.status = "Exited";
                usedRAM -= c.memoryLimit;
                usedDisk -= c.diskLimit;
                std::cout << "Container " << name << " stopped. Resources released." << std::endl;
                saveState();
                return;
            }
        }
        std::cout << "Container " << name << " not found." << std::endl;
    }

    void saveState() {
        std::ofstream ofs(STATE_FILE);
        for (const auto& c : activeContainers) {
            ofs << c.name << " " << c.status << " " << c.pid << " " << c.memoryLimit << " " << c.cpuPriority << " " << c.diskLimit << "\n";
        }
    }

    void loadState() {
        activeContainers.clear();
        usedRAM = 0; usedDisk = 0;
        if (!fs::exists(STATE_FILE)) return;
        std::ifstream ifs(STATE_FILE);
        ContainerInfo c;
        while (ifs >> c.name >> c.status >> c.pid >> c.memoryLimit >> c.cpuPriority >> c.diskLimit) {
            activeContainers.push_back(c);
            if (c.status == "Running") {
                usedRAM += c.memoryLimit;
                usedDisk += c.diskLimit;
            }
        }
    }

    void waitForContainers() {
#ifdef _WIN32
        std::cout << "[Host] Windows simulation: All containers processed." << std::endl;
#else
        int status;
        pid_t p;
        while ((p = wait(&status)) > 0);
#endif
    }
};

#endif
