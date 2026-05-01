#ifndef IPC_MANAGER_H
#define IPC_MANAGER_H

#include <iostream>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

class IPCManager {
public:
    void simulatePipe() {
#ifdef _WIN32
        std::cout << "[Host] Windows simulation: Pipe IPC simulated using memory buffer." << std::endl;
        const char* msg = "Hello from Host (Windows)!";
        char buffer[100];
        strcpy(buffer, msg);
        std::cout << "[Child] Received via simulated pipe: " << buffer << std::endl;
#else
        int pipefd[2];
        char buffer[100];

        if (pipe(pipefd) == -1) {
            std::cerr << "Pipe failed" << std::endl;
            return;
        }

        pid_t pid = fork();

        if (pid == 0) {
            // Child: Reader
            close(pipefd[1]); // Close write end
            read(pipefd[0], buffer, sizeof(buffer));
            std::cout << "[Child] Received via pipe: " << buffer << std::endl;
            close(pipefd[0]);
            exit(0);
        } else {
            // Parent: Writer
            close(pipefd[0]); // Close read end
            const char* msg = "Hello from Host!";
            write(pipefd[1], msg, strlen(msg) + 1);
            std::cout << "[Host] Sent via pipe: " << msg << std::endl;
            close(pipefd[1]);
        }
#endif
    }
};

#endif
