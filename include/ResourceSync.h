#ifndef RESOURCE_SYNC_H
#define RESOURCE_SYNC_H

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>

typedef struct {
    HANDLE sem;
} WinSemaphore;

WinSemaphore* WinSemaphore_Create(int initial);
void WinSemaphore_Destroy(WinSemaphore* ws);
void WinSemaphore_Wait(WinSemaphore* ws);
void WinSemaphore_Post(WinSemaphore* ws);

#else
#include <semaphore.h>
#include <pthread.h>

typedef struct {
    sem_t sem;
} UnixSemaphore;

UnixSemaphore* UnixSemaphore_Create(int initial);
void UnixSemaphore_Destroy(UnixSemaphore* us);
void UnixSemaphore_Wait(UnixSemaphore* us);
void UnixSemaphore_Post(UnixSemaphore* us);
#endif

typedef struct {
#ifdef _WIN32
    CRITICAL_SECTION fileMutex;
    WinSemaphore* deviceSemaphore;
#else
    pthread_mutex_t fileMutex;
    UnixSemaphore* deviceSemaphore;
#endif
} ResourceSync;

ResourceSync* ResourceSync_Create(void);
void ResourceSync_Destroy(ResourceSync* rs);
void ResourceSync_AccessSharedFile(ResourceSync* rs, int containerId);
void ResourceSync_UseSharedDevice(ResourceSync* rs, int containerId);

#endif
