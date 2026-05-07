#include "../include/ContainerManager.h"
#include <stdio.h>
#include <string.h>

ContainerManager* ContainerManager_Create(void) {
    ContainerManager* cm = (ContainerManager*)malloc(sizeof(ContainerManager));
    if (!cm) return NULL;
    
    cm->containerCount = 0;
    cm->totalRAM = 8192;
    cm->totalDisk = 50000;
    cm->usedRAM = 0;
    cm->usedDisk = 0;
    
    ContainerManager_LoadState(cm);
    return cm;
}

void ContainerManager_Destroy(ContainerManager* cm) {
    if (cm) free(cm);
}

void ContainerManager_SetGlobalResources(ContainerManager* cm, int budget, int capacity) {
    if (!cm) return;
    cm->totalRAM = budget;
    cm->totalDisk = capacity;
    printf("[Host] Marketing Resources Set: Budget=$%dK, Capacity=%d units\n", cm->totalRAM, cm->totalDisk);
}

void ContainerManager_CreateContainer(ContainerManager* cm, const char* containerName, 
                                      const char* taskData, int memLimit, int diskLimit, int priority) {
    if (!cm || !containerName || cm->containerCount >= MAX_CONTAINERS) return;
    
    // Resource validation
    if (cm->usedRAM + memLimit > cm->totalRAM) {
        fprintf(stderr, "Error: Not enough budget! Required: $%dK, Available: $%dK\n", 
                memLimit, cm->totalRAM - cm->usedRAM);
        return;
    }
    if (cm->usedDisk + diskLimit > cm->totalDisk) {
        fprintf(stderr, "Error: Not enough capacity! Required: %d units, Available: %d units\n", 
                diskLimit, cm->totalDisk - cm->usedDisk);
        return;
    }
    
    // Create campaigns directory and workspace
    char workspace[768];
    snprintf(workspace, sizeof(workspace) - 1, "campaigns/%s", containerName);
    
    // Use a large enough buffer to avoid truncation warnings
#ifdef _WIN32
    char mkdirCmd[2048];
    snprintf(mkdirCmd, sizeof(mkdirCmd), "if not exist \"campaigns\" mkdir \"campaigns\" && if not exist \"%s\" mkdir \"%s\"", workspace, workspace);
    system(mkdirCmd);
#else
    char mkdirCmd[2048];
    snprintf(mkdirCmd, sizeof(mkdirCmd), "mkdir -p \"%s\"", workspace);
    system(mkdirCmd);
#endif
    
    // Save task info to file
    char taskFile[1024];
    snprintf(taskFile, sizeof(taskFile) - 1, "%s/task.json", workspace);
    FILE* f = fopen(taskFile, "w");
    if (f) {
        fprintf(f, "{\n  \"name\": \"%s\",\n  \"task\": \"%s\",\n  \"mem\": %d,\n  \"disk\": %d,\n  \"priority\": %d\n}\n",
                containerName, taskData, memLimit, diskLimit, priority);
        fclose(f);
    }
    
    // Add to containers list
    ContainerInfo* c = &cm->activeContainers[cm->containerCount];
    strncpy(c->name, containerName, sizeof(c->name) - 1);
    c->name[sizeof(c->name) - 1] = '\0';
    strcpy(c->status, "Running");
    c->pid = 100 + cm->containerCount;
    c->memoryLimit = memLimit;
    c->cpuPriority = priority;
    c->diskLimit = diskLimit;
    
    cm->usedRAM += memLimit;
    cm->usedDisk += diskLimit;
    cm->containerCount++;
    
    ContainerManager_SaveState(cm);
    printf("[Host] Campaign %s successfully deployed.\n", containerName);
}

void ContainerManager_ListContainers(ContainerManager* cm) {
    if (!cm) return;
    
    printf("\nSYSTEM LOAD: [BUDGET: $%dK/$%dK] [CAPACITY: %d/%d units]\n", 
           cm->usedRAM, cm->totalRAM, cm->usedDisk, cm->totalDisk);
    printf("--------------------------------------------------------------------------------\n");
    printf("CAMPAIGN ID\tAGENCY\t\tSTATUS\t\tBUDGET\tCAP\tPRIO\n");
    printf("--------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < cm->containerCount; i++) {
        ContainerInfo* c = &cm->activeContainers[i];
        printf("%d\t\t%s\t\t%s\t\t$%dK\t%d\t%d\n", 
               c->pid, c->name, c->status, c->memoryLimit, c->diskLimit, c->cpuPriority);
    }
}

void ContainerManager_StopContainer(ContainerManager* cm, const char* name) {
    if (!cm || !name) return;
    
    for (int i = 0; i < cm->containerCount; i++) {
        if (strcmp(cm->activeContainers[i].name, name) == 0) {
            ContainerInfo* c = &cm->activeContainers[i];
            if (strcmp(c->status, "Exited") == 0) {
                printf("Campaign for %s is already paused.\n", name);
                return;
            }
            strcpy(c->status, "Exited");
            cm->usedRAM -= c->memoryLimit;
            cm->usedDisk -= c->diskLimit;
            printf("Campaign for %s paused. Resources released.\n", name);
            ContainerManager_SaveState(cm);
            return;
        }
    }
    printf("Container %s not found.\n", name);
}

void ContainerManager_SaveState(ContainerManager* cm) {
    if (!cm) return;
    
    // Ensure campaigns directory exists for state persistence
#ifdef _WIN32
    system("if not exist \"campaigns\" mkdir \"campaigns\"");
#else
    system("mkdir -p campaigns");
#endif
    
    FILE* f = fopen(STATE_FILE, "w");
    if (!f) return;
    
    for (int i = 0; i < cm->containerCount; i++) {
        ContainerInfo* c = &cm->activeContainers[i];
        fprintf(f, "%s %s %d %d %d %d\n", c->name, c->status, c->pid, c->memoryLimit, c->cpuPriority, c->diskLimit);
    }
    fclose(f);
}

void ContainerManager_LoadState(ContainerManager* cm) {
    if (!cm) return;
    
    cm->containerCount = 0;
    cm->usedRAM = 0;
    cm->usedDisk = 0;
    
    FILE* f = fopen(STATE_FILE, "r");
    if (!f) return;
    
    while (cm->containerCount < MAX_CONTAINERS) {
        ContainerInfo* c = &cm->activeContainers[cm->containerCount];
        int result = fscanf(f, "%255s %49s %d %d %d %d\n", 
                           c->name, c->status, &c->pid, &c->memoryLimit, &c->cpuPriority, &c->diskLimit);
        if (result != 6) break;
        
        if (strcmp(c->status, "Running") == 0) {
            cm->usedRAM += c->memoryLimit;
            cm->usedDisk += c->diskLimit;
        }
        cm->containerCount++;
    }
    fclose(f);
}

void ContainerManager_WaitForContainers(ContainerManager* cm) {
    if (!cm) return;
    
#ifdef _WIN32
    printf("[Host] Windows simulation: All containers processed.\n");
#else
    int status;
    pid_t p;
    while ((p = wait(&status)) > 0);
#endif
}
