#ifndef CONTAINER_MANAGER_H
#define CONTAINER_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
typedef int pid_t;
#else
#include <unistd.h>
#include <sys/wait.h>
#endif

#define MAX_CONTAINERS 256
#define STATE_FILE "containers/engine_state.dat"

typedef struct {
    char name[256];
    char status[50];
    int pid;
    int memoryLimit;
    int cpuPriority;
    int diskLimit;
} ContainerInfo;

typedef struct {
    ContainerInfo activeContainers[MAX_CONTAINERS];
    int containerCount;
    int totalRAM;
    int totalDisk;
    int usedRAM;
    int usedDisk;
} ContainerManager;

ContainerManager* ContainerManager_Create(void);
void ContainerManager_Destroy(ContainerManager* cm);
void ContainerManager_SetGlobalResources(ContainerManager* cm, int ram, int disk);
void ContainerManager_CreateContainer(ContainerManager* cm, const char* containerName, 
                                      const char* taskData, int memLimit, int diskLimit, int priority);
void ContainerManager_ListContainers(ContainerManager* cm);
void ContainerManager_StopContainer(ContainerManager* cm, const char* name);
void ContainerManager_SaveState(ContainerManager* cm);
void ContainerManager_LoadState(ContainerManager* cm);
void ContainerManager_WaitForContainers(ContainerManager* cm);

#endif
