#ifndef RESOURCE_SYNC_H
#define RESOURCE_SYNC_H

#include <mutex>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
// Simple wrapper for semaphore simulation on Windows if POSIX semaphore.h is missing
class WinSemaphore {
    HANDLE sem;
public:
    WinSemaphore(int initial) { sem = CreateSemaphore(NULL, initial, 10, NULL); }
    ~WinSemaphore() { CloseHandle(sem); }
    void wait() { WaitForSingleObject(sem, INFINITE); }
    void post() { ReleaseSemaphore(sem, 1, NULL); }
};
#else
#include <semaphore.h>
#endif

class ResourceSync {
    std::mutex fileMutex;
#ifdef _WIN32
    WinSemaphore deviceSemaphore;
#else
    sem_t deviceSemaphore;
#endif

public:
    ResourceSync() 
#ifdef _WIN32
        : deviceSemaphore(1) 
#endif
    {
#ifndef _WIN32
        sem_init(&deviceSemaphore, 0, 1); // Binary semaphore
#endif
    }

    ~ResourceSync() {
#ifndef _WIN32
        sem_destroy(&deviceSemaphore);
#endif
    }

    void accessSharedFile(int containerId) {
        std::lock_guard<std::mutex> lock(fileMutex);
        std::cout << "Container " << containerId << " is writing to shared file." << std::endl;
        // Simulate file I/O
    }

    void useSharedDevice(int containerId) {
#ifdef _WIN32
        deviceSemaphore.wait();
#else
        sem_wait(&deviceSemaphore);
#endif
        std::cout << "Container " << containerId << " is using shared device." << std::endl;
        // Simulate device usage
#ifdef _WIN32
        deviceSemaphore.post();
#else
        sem_post(&deviceSemaphore);
#endif
    }
};

#endif
