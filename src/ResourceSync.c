#include "../include/ResourceSync.h"
#include <stdio.h>

#ifdef _WIN32

WinSemaphore* WinSemaphore_Create(int initial) {
    WinSemaphore* ws = (WinSemaphore*)malloc(sizeof(WinSemaphore));
    if (!ws) return NULL;
    ws->sem = CreateSemaphore(NULL, initial, 10, NULL);
    return ws;
}

void WinSemaphore_Destroy(WinSemaphore* ws) {
    if (ws) {
        if (ws->sem) CloseHandle(ws->sem);
        free(ws);
    }
}

void WinSemaphore_Wait(WinSemaphore* ws) {
    if (ws && ws->sem) {
        WaitForSingleObject(ws->sem, INFINITE);
    }
}

void WinSemaphore_Post(WinSemaphore* ws) {
    if (ws && ws->sem) {
        ReleaseSemaphore(ws->sem, 1, NULL);
    }
}

#else

UnixSemaphore* UnixSemaphore_Create(int initial) {
    UnixSemaphore* us = (UnixSemaphore*)malloc(sizeof(UnixSemaphore));
    if (!us) return NULL;
    sem_init(&us->sem, 0, initial);
    return us;
}

void UnixSemaphore_Destroy(UnixSemaphore* us) {
    if (us) {
        sem_destroy(&us->sem);
        free(us);
    }
}

void UnixSemaphore_Wait(UnixSemaphore* us) {
    if (us) {
        sem_wait(&us->sem);
    }
}

void UnixSemaphore_Post(UnixSemaphore* us) {
    if (us) {
        sem_post(&us->sem);
    }
}

#endif

ResourceSync* ResourceSync_Create(void) {
    ResourceSync* rs = (ResourceSync*)malloc(sizeof(ResourceSync));
    if (!rs) return NULL;
    
#ifdef _WIN32
    InitializeCriticalSection(&rs->fileMutex);
    rs->deviceSemaphore = WinSemaphore_Create(1);
#else
    pthread_mutex_init(&rs->fileMutex, NULL);
    rs->deviceSemaphore = UnixSemaphore_Create(1);
#endif
    
    return rs;
}

void ResourceSync_Destroy(ResourceSync* rs) {
    if (!rs) return;
    
#ifdef _WIN32
    DeleteCriticalSection(&rs->fileMutex);
    WinSemaphore_Destroy(rs->deviceSemaphore);
#else
    pthread_mutex_destroy(&rs->fileMutex);
    UnixSemaphore_Destroy(rs->deviceSemaphore);
#endif
    
    free(rs);
}

void ResourceSync_AccessSharedFile(ResourceSync* rs, int containerId) {
    if (!rs) return;
    
#ifdef _WIN32
    EnterCriticalSection(&rs->fileMutex);
#else
    pthread_mutex_lock(&rs->fileMutex);
#endif
    
    printf("Container %d is writing to shared file.\n", containerId);
    
#ifdef _WIN32
    LeaveCriticalSection(&rs->fileMutex);
#else
    pthread_mutex_unlock(&rs->fileMutex);
#endif
}

void ResourceSync_UseSharedDevice(ResourceSync* rs, int containerId) {
    if (!rs || !rs->deviceSemaphore) return;
    
#ifdef _WIN32
    WinSemaphore_Wait(rs->deviceSemaphore);
#else
    UnixSemaphore_Wait(rs->deviceSemaphore);
#endif
    
    printf("Container %d is using shared device.\n", containerId);
    
#ifdef _WIN32
    WinSemaphore_Post(rs->deviceSemaphore);
#else
    UnixSemaphore_Post(rs->deviceSemaphore);
#endif
}
