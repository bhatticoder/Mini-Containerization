#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/Process.h"
#include "../include/RoundRobinScheduler.h"
#include "../include/ContainerManager.h"
#include "../include/ResourceSync.h"
#include "../include/IPCManager.h"
#include "../include/BankersAlgorithm.h"
#include "../include/MemoryManager.h"
#include "../include/FIFOMemoryManager.h"
#include "../include/FCFSScheduler.h"

#define MAX_INPUT 1024
#define MAX_TOKENS 64

void printHelp() {
    printf("\n==============================================\n");
    printf("    LITECONTAINER ENGINE - DOCKER CLI\n");
    printf("==============================================\n");
    printf("COMMANDS:\n");
    printf("  config <ram_mb> <disk_mb>           - Set total system resources\n");
    printf("  run <name> <task> [-m mem] [-d disk] - Create container with quotas\n");
    printf("  ps                                  - List containers & usage\n");
    printf("  stop <name>                         - Stop container\n");
    printf("  stats/compare                       - Benchmarking & Simulation Reports\n");
    printf("  sync                                - Resource sync demo\n");
    printf("  help                                - Show help\n");
    printf("  exit                                - Exit\n");
    printf("==============================================\n\n");
}

typedef struct {
    char* tokens[MAX_TOKENS];
    int count;
} TokenList;

void tokenize(const char* line, TokenList* tokens) {
    tokens->count = 0;
    char linecopy[MAX_INPUT];
    strncpy(linecopy, line, sizeof(linecopy) - 1);
    linecopy[sizeof(linecopy) - 1] = '\0';
    
    char* token = strtok(linecopy, " ");
    while (token && tokens->count < MAX_TOKENS) {
        char* newtoken = (char*)malloc(strlen(token) + 1);
        strcpy(newtoken, token);
        tokens->tokens[tokens->count++] = newtoken;
        token = strtok(NULL, " ");
    }
}

void freeTokens(TokenList* tokens) {
    for (int i = 0; i < tokens->count; i++) {
        free(tokens->tokens[i]);
    }
}

int main() {
    ContainerManager* cm = ContainerManager_Create();
    printHelp();
    
    char line[MAX_INPUT];
    
    while (1) {
        printf("docker-lite> ");
        
        if (!fgets(line, sizeof(line), stdin)) break;
        
        // Remove newline
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
        }
        
        if (strcmp(line, "exit") == 0) break;
        
        TokenList tokens;
        tokenize(line, &tokens);
        
        if (tokens.count == 0) {
            continue;
        }
        
        const char* cmd = tokens.tokens[0];
        
        if (strcmp(cmd, "config") == 0) {
            if (tokens.count >= 3) {
                int r = atoi(tokens.tokens[1]);
                int d = atoi(tokens.tokens[2]);
                ContainerManager_SetGlobalResources(cm, r, d);
            } else {
                printf("Usage: config <ram_mb> <disk_mb>\n");
            }
        }
        else if (strcmp(cmd, "run") == 0) {
            if (tokens.count >= 3) {
                const char* name = tokens.tokens[1];
                const char* task = tokens.tokens[2];
                int mem = 256, disk = 1000, prio = 1;
                
                // Parse options
                for (int i = 3; i < tokens.count - 1; i++) {
                    if (strcmp(tokens.tokens[i], "-m") == 0) {
                        mem = atoi(tokens.tokens[i+1]);
                    } else if (strcmp(tokens.tokens[i], "-d") == 0) {
                        disk = atoi(tokens.tokens[i+1]);
                    } else if (strcmp(tokens.tokens[i], "-p") == 0) {
                        prio = atoi(tokens.tokens[i+1]);
                    }
                }
                
                ContainerManager_CreateContainer(cm, name, task, mem, disk, prio);
            } else {
                printf("Error: run requires an image/name and task.\n");
            }
        }
        else if (strcmp(cmd, "ps") == 0) {
            ContainerManager_ListContainers(cm);
        }
        else if (strcmp(cmd, "stop") == 0) {
            if (tokens.count >= 2) {
                ContainerManager_StopContainer(cm, tokens.tokens[1]);
            } else {
                printf("Usage: stop <name>\n");
            }
        }
        else if (strcmp(cmd, "clear") == 0) {
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            printHelp();
        }
        else if (strcmp(cmd, "stats") == 0 || strcmp(cmd, "compare") == 0 || strcmp(cmd, "state") == 0) {
            printf("\n[Module 2/6] ALGORITHM COMPARISON REPORT\n");
            
            // Scheduling comparison
            printf("\n--- CPU Scheduling Comparison ---\n");
            Process pbatch1[2];
            pbatch1[0] = *Process_Create(1, "Job1", 0, 10, 0);
            pbatch1[1] = *Process_Create(2, "Job2", 2, 5, 0);
            
            printf(">> FCFS Strategy:\n");
            FCFSScheduler* fcfs = FCFSScheduler_Create();
            fcfs->base.schedule((Scheduler*)fcfs, pbatch1, 2);
            fcfs->base.destroy((Scheduler*)fcfs);
            
            Process pbatch2[2];
            pbatch2[0] = *Process_Create(1, "Job1", 0, 10, 0);
            pbatch2[1] = *Process_Create(2, "Job2", 2, 5, 0);
            
            printf("\n>> Round Robin (Quantum=3) Strategy:\n");
            RoundRobinScheduler* rr = RoundRobinScheduler_Create(3);
            rr->base.schedule((Scheduler*)rr, pbatch2, 2);
            rr->base.destroy((Scheduler*)rr);
            
            // Memory comparison
            printf("\n--- Memory Page Replacement Comparison ---\n");
            int stream[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
            int stream_size = sizeof(stream) / sizeof(stream[0]);
            
            MemoryManager* lru = MemoryManager_Create(3);
            FIFOMemoryManager* fifo = FIFOMemoryManager_Create(3);
            
            for (int i = 0; i < stream_size; i++) {
                MemoryManager_AccessPage(lru, stream[i]);
                FIFOMemoryManager_AccessPage(fifo, stream[i]);
            }
            
            printf("LRU Page Faults: %d\n", MemoryManager_GetPageFaults(lru));
            printf("FIFO Page Faults: %d\n", FIFOMemoryManager_GetPageFaults(fifo));
            
            MemoryManager_Destroy(lru);
            FIFOMemoryManager_Destroy(fifo);
        }
        else if (strcmp(cmd, "sync") == 0) {
            ResourceSync* sync = ResourceSync_Create();
            ResourceSync_AccessSharedFile(sync, 1);
            ResourceSync_UseSharedDevice(sync, 2);
            ResourceSync_Destroy(sync);
        }
        else if (strcmp(cmd, "help") == 0) {
            printHelp();
        }
        else {
            printf("Unknown command: %s\n", cmd);
        }
        
        freeTokens(&tokens);
    }
    
    ContainerManager_Destroy(cm);
    return 0;
}

