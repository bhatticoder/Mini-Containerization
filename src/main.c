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
    printf("    B2B MARKETING CAMPAIGN SYSTEM\n");
    printf("==============================================\n");
    printf("COMMANDS:\n");
    printf("  config <budget> <capacity>          - Set global marketing resources\n");
    printf("  launch <agency> <campaign> [-b budget] [-c capacity] - Launch campaign with quotas\n");
    printf("  campaigns                           - List campaigns & resource usage\n");
    printf("  pause <agency>                      - Pause campaign execution\n");
    printf("  analytics                           - Campaign performance reports\n");
    printf("  sync                                - Resource synchronization demo\n");
    printf("  help                                - Show help\n");
    printf("  exit                                - Exit system\n");
    printf("==============================================\n\n");
}

typedef struct {
    char* tokens[MAX_TOKENS];
    int count;
} TokenList;

void trimWhitespace(char* str) {
    if (!str) return;
    size_t start = 0;
    size_t end = strlen(str);

    while (start < end && (str[start] == ' ' || str[start] == '\t' || str[start] == '\n' || str[start] == '\r')) {
        start++;
    }
    while (end > start && (str[end-1] == ' ' || str[end-1] == '\t' || str[end-1] == '\n' || str[end-1] == '\r')) {
        end--;
    }
    if (start > 0) {
        memmove(str, str + start, end - start);
    }
    str[end - start] = '\0';
}

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
        printf("marketing-sim> ");
        
        if (!fgets(line, sizeof(line), stdin)) break;
        
        trimWhitespace(line);
        
        if (strcmp(line, "exit") == 0) break;
        
        TokenList tokens;
        tokenize(line, &tokens);
        
        if (tokens.count == 0) {
            continue;
        }
        
        const char* cmd = tokens.tokens[0];
        
        if (strcmp(cmd, "config") == 0) {
            if (tokens.count >= 3) {
                int budget = atoi(tokens.tokens[1]);
                int capacity = atoi(tokens.tokens[2]);
                ContainerManager_SetGlobalResources(cm, budget, capacity);
                printf("[System] Marketing Resources Set: Budget=$%dK, Capacity=%d\n", budget, capacity);
            } else {
                printf("Usage: config <budget_$1000s> <team_capacity>\n");
            }
        }
        else if (strcmp(cmd, "launch") == 0) {
            if (tokens.count >= 3) {
                const char* agency = tokens.tokens[1];
                const char* campaign = tokens.tokens[2];
                int budget = 50, capacity = 10, prio = 1;
                
                // Parse options
                for (int i = 3; i < tokens.count - 1; i++) {
                    if (strcmp(tokens.tokens[i], "-b") == 0) {
                        budget = atoi(tokens.tokens[i+1]);
                    } else if (strcmp(tokens.tokens[i], "-c") == 0) {
                        capacity = atoi(tokens.tokens[i+1]);
                    } else if (strcmp(tokens.tokens[i], "-p") == 0) {
                        prio = atoi(tokens.tokens[i+1]);
                    }
                }
                
                ContainerManager_CreateContainer(cm, agency, campaign, budget, capacity, prio);
                printf("[System] Campaign '%s' launched by agency '%s'.\n", campaign, agency);
            } else {
                printf("Error: launch requires an agency name and campaign type.\n");
            }
        }
        else if (strcmp(cmd, "campaigns") == 0 || strcmp(cmd, "ps") == 0) {
            ContainerManager_ListContainers(cm);
        }
        else if (strcmp(cmd, "pause") == 0 || strcmp(cmd, "stop") == 0) {
            if (tokens.count >= 2) {
                ContainerManager_StopContainer(cm, tokens.tokens[1]);
                printf("[System] Campaign paused for agency '%s'.\n", tokens.tokens[1]);
            } else {
                printf("Usage: pause <agency>\n");
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
        else if (strcmp(cmd, "analytics") == 0 || strcmp(cmd, "stats") == 0 || strcmp(cmd, "compare") == 0 || strcmp(cmd, "report") == 0) {
            printf("\n[Marketing Analytics] Campaign Scheduling & Resource Safety Report\n");
            
            // Scheduling comparison
            printf("\n--- CPU Scheduling Comparison ---\n");
            Process* campaignA = Process_Create(1, "CampaignA", 0, 8, 1);
            Process* campaignB = Process_Create(2, "CampaignB", 1, 6, 2);
            Process campaignBatch1[2];
            campaignBatch1[0] = *campaignA;
            campaignBatch1[1] = *campaignB;
            Process_Destroy(campaignA);
            Process_Destroy(campaignB);
            
            printf(">> FCFS Campaign Scheduling:\n");
            FCFSScheduler* fcfs = FCFSScheduler_Create();
            fcfs->base.schedule((Scheduler*)fcfs, campaignBatch1, 2);
            fcfs->base.destroy((Scheduler*)fcfs);
            
            Process* campaignC = Process_Create(3, "CampaignA", 0, 8, 1);
            Process* campaignD = Process_Create(4, "CampaignB", 1, 6, 2);
            Process campaignBatch2[2];
            campaignBatch2[0] = *campaignC;
            campaignBatch2[1] = *campaignD;
            Process_Destroy(campaignC);
            Process_Destroy(campaignD);
            
            printf("\n>> Round Robin Campaign Scheduling (Quantum=3):\n");
            RoundRobinScheduler* rr = RoundRobinScheduler_Create(3);
            rr->base.schedule((Scheduler*)rr, campaignBatch2, 2);
            rr->base.destroy((Scheduler*)rr);
            
            // Memory comparison for analytics cache
            printf("\n--- Marketing Analytics Cache Comparison ---\n");
            int pageStream[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
            int streamSize = sizeof(pageStream) / sizeof(pageStream[0]);
            
            MemoryManager* lru = MemoryManager_Create(3);
            FIFOMemoryManager* fifo = FIFOMemoryManager_Create(3);
            
            for (int i = 0; i < streamSize; i++) {
                MemoryManager_AccessPage(lru, pageStream[i]);
                FIFOMemoryManager_AccessPage(fifo, pageStream[i]);
            }
            
            printf("LRU Page Faults: %d\n", MemoryManager_GetPageFaults(lru));
            printf("FIFO Page Faults: %d\n", FIFOMemoryManager_GetPageFaults(fifo));
            
            MemoryManager_Destroy(lru);
            FIFOMemoryManager_Destroy(fifo);
            
            // Banker's Algorithm for campaign resource safety
            printf("\n--- Deadlock Prevention (Banker's Algorithm) ---\n");
            BankersAlgorithm* bankers = BankersAlgorithm_Create(3, 2);
            int available[2] = { cm->totalRAM - cm->usedRAM, cm->totalDisk - cm->usedDisk };
            BankersAlgorithm_SetAvailable(bankers, available);
            
            int alloc0[2] = {20, 5};
            int alloc1[2] = {30, 8};
            int alloc2[2] = {10, 3};
            BankersAlgorithm_SetAllocation(bankers, 0, alloc0);
            BankersAlgorithm_SetAllocation(bankers, 1, alloc1);
            BankersAlgorithm_SetAllocation(bankers, 2, alloc2);
            
            int max0[2] = {25, 6};
            int max1[2] = {35, 10};
            int max2[2] = {15, 5};
            BankersAlgorithm_SetMaxNeed(bankers, 0, max0);
            BankersAlgorithm_SetMaxNeed(bankers, 1, max1);
            BankersAlgorithm_SetMaxNeed(bankers, 2, max2);
            
            bool safe = BankersAlgorithm_IsSafe(bankers);
            printf("Resource allocation state is %s.\n", safe ? "SAFE" : "UNSAFE");
            BankersAlgorithm_Destroy(bankers);
            
            // Synchronization simulation for shared marketing resources
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

