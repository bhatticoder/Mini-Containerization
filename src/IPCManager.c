#include "../include/IPCManager.h"

IPCManager* IPCManager_Create(void) {
    IPCManager* ipc = (IPCManager*)malloc(sizeof(IPCManager));
    if (!ipc) return NULL;
    ipc->dummy = 0;
    return ipc;
}

void IPCManager_Destroy(IPCManager* ipc) {
    if (ipc) free(ipc);
}

void IPCManager_SimulatePipe(IPCManager* ipc) {
    if (!ipc) return;
    
#ifdef _WIN32
    printf("[Host] Windows simulation: Pipe IPC simulated using memory buffer.\n");
    const char* msg = "Hello from Host (Windows)!";
    char buffer[100];
    strcpy(buffer, msg);
    printf("[Child] Received via simulated pipe: %s\n", buffer);
#else
    int pipefd[2];
    char buffer[100];
    
    if (pipe(pipefd) == -1) {
        fprintf(stderr, "Pipe failed\n");
        return;
    }
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // Child: Reader
        close(pipefd[1]);
        read(pipefd[0], buffer, sizeof(buffer));
        printf("[Child] Received via pipe: %s\n", buffer);
        close(pipefd[0]);
        exit(0);
    } else {
        // Parent: Writer
        close(pipefd[0]);
        const char* msg = "Hello from Host!";
        write(pipefd[1], msg, strlen(msg) + 1);
        printf("[Host] Sent via pipe: %s\n", msg);
        close(pipefd[1]);
    }
#endif
}
