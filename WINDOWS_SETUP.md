# LiteContainer Engine - C Project (Windows Setup)

## Project Overview
This is the **OS Project LiteContainer Engine** - a container simulation system with CPU scheduling algorithms, memory management, and resource synchronization. Originally developed for Linux, it has been converted to C with Windows/Linux compatibility.

## Prerequisites

### Required for Windows:
1. **MinGW (GCC for Windows)** - [Download](https://www.mingw-w64.org/)
   - Install MinGW with gcc, g++, and make
   - Add to system PATH
   
2. **Visual Studio Code** - [Download](https://code.visualstudio.com/)

3. **VS Code Extensions:**
   - C/C++ (Microsoft official)
   - C/C++ Compile Run (Daniel Phou)
   - Code Runner (Jun Han)

### Verify Installation:
```powershell
gcc --version
gdb --version
```

## Project Structure

```
├── bin/                          # Compiled executables
│   └── lite_container.exe       # Main executable
├── include/                      # Header files
│   ├── Process.h
│   ├── Scheduler.h
│   ├── FCFSScheduler.h
│   ├── RoundRobinScheduler.h
│   ├── MemoryManager.h
│   ├── FIFOMemoryManager.h
│   ├── ContainerManager.h
│   ├── ResourceSync.h
│   ├── IPCManager.h
│   └── BankersAlgorithm.h
├── src/                          # C Source files
│   ├── main.c
│   ├── Process.c
│   ├── Scheduler.c
│   ├── FCFSScheduler.c
│   ├── RoundRobinScheduler.c
│   ├── MemoryManager.c
│   ├── FIFOMemoryManager.c
│   ├── ContainerManager.c
│   ├── ResourceSync.c
│   ├── IPCManager.c
│   └── BankersAlgorithm.c
├── containers/                   # Container workspace
└── .vscode/                      # VS Code configuration
    ├── tasks.json               # Build tasks
    ├── launch.json              # Debug configuration
    └── c_cpp_properties.json    # IntelliSense settings
```

## Building the Project

### Method 1: VS Code Build Task (Recommended)
1. Press `Ctrl + Shift + B` to build
2. Select **"Build Lite Container"** task
3. Output executable: `bin/lite_container.exe`

### Method 2: Manual Command (PowerShell)
```powershell
cd "g:\Fast\Semester 6\OS Project"
gcc -I./include src/*.c -o bin/lite_container.exe -Wall -std=c99
```

### Method 3: Command Line Compile All
```powershell
gcc -I./include `
    src/Process.c `
    src/Scheduler.c `
    src/FCFSScheduler.c `
    src/RoundRobinScheduler.c `
    src/MemoryManager.c `
    src/FIFOMemoryManager.c `
    src/ContainerManager.c `
    src/ResourceSync.c `
    src/IPCManager.c `
    src/BankersAlgorithm.c `
    src/main.c `
    -o bin/lite_container.exe -Wall -std=c99
```

## Running the Project

### Run from VS Code
1. Press `F5` to debug (requires build first)
2. Or use task: `Ctrl + Shift + B` → **"Run Lite Container"**

### Run from PowerShell
```powershell
.\bin\lite_container.exe
```

### Run with Commands
```powershell
@("config 2048 10000", "ps", "stats", "exit") | .\bin\lite_container.exe
```

## Available Commands

```
config <ram_mb> <disk_mb>           - Set total system resources
run <name> <task> [-m mem] [-d disk] - Create container with quotas
ps                                  - List containers & usage
stop <name>                         - Stop container
stats/compare                       - Algorithm comparison report
sync                                - Resource synchronization demo
help                                - Show help
exit                                - Exit program
```

## Features

### 1. **CPU Scheduling Algorithms**
- FCFS (First Come First Served)
- Round Robin with configurable quantum
- Metrics: waiting time, turnaround time

### 2. **Memory Management**
- LRU (Least Recently Used) page replacement
- FIFO (First In First Out) page replacement
- Page fault counting

### 3. **Container Management**
- Create containers with resource quotas
- Resource allocation (RAM/Disk)
- Container lifecycle management (Running/Exited)

### 4. **Deadlock Prevention**
- Banker's Algorithm implementation
- Resource safety checking

### 5. **Resource Synchronization**
- Mutex locks (cross-platform)
- Semaphores (Windows/Unix compatible)
- Shared file access control

## Debugging in VS Code

### Setting Breakpoints
1. Click on the line number to add a breakpoint (red dot)
2. Press `F5` to start debugging
3. Use debug controls to step through code

### Debug Configuration
The project includes a debug configuration (`launch.json`) that:
- Uses GDB debugger
- Sets breakpoints at startup
- Shows variable values in hover tooltips
- Provides call stack information

## Troubleshooting

### Issue: `gcc: command not found`
**Solution:** MinGW not installed or not in PATH
- Verify: `gcc --version`
- Add MinGW bin folder to Windows PATH environment variable
- Restart VS Code

### Issue: `include/header.h: No such file or directory`
**Solution:** Ensure `-I./include` flag in build command
- Check `.vscode/tasks.json` has this argument
- Rebuild project

### Issue: Compilation errors with `windows.h`
**Solution:** Platform-specific code is wrapped with `#ifdef _WIN32`
- On Windows: Windows code is used
- This is intentional - project works on both Windows and Linux

### Issue: `bin/lite_container.exe` not found after build
**Solution:** Check compiler output for errors
- Open VS Code terminal: `` Ctrl + ` ``
- Check for error messages
- Ensure all source files are listed in build command

## Windows vs Linux Differences

### Handled Automatically:
| Feature | Windows | Linux |
|---------|---------|-------|
| File paths | `\` | `/` |
| System calls | `CreateSemaphore`, `CreateThread` | `sem_init`, `fork` |
| Mutexes | `CRITICAL_SECTION` | `pthread_mutex` |
| Containers | Simulated (no fork) | Real processes (fork) |

The code uses `#ifdef _WIN32` guards to handle platform differences automatically.

## Example Usage

### Example 1: Basic Container Operations
```
docker-lite> config 2048 10000
[Host] System Resources Set: RAM=2048MB, Disk=10000MB

docker-lite> run web-server docker-image -m 512 -d 2000
[Host] Container web-server successfully deployed.

docker-lite> ps
SYSTEM LOAD: [RAM: 512/2048MB] [DISK: 2000/10000MB]
CONTAINER ID    NAME            STATUS    MEM    DISK    PRIO
100              web-server      Running   512M   2000M   1

docker-lite> stop web-server
Container web-server stopped. Resources released.

docker-lite> exit
```

### Example 2: Algorithm Comparison
```
docker-lite> stats

[Module 2/6] ALGORITHM COMPARISON REPORT

--- CPU Scheduling Comparison ---
>> FCFS Strategy:
Time 0: Starting Job1 (Burst: 10)
Time 10: Starting Job2 (Burst: 5)
Average Waiting Time: 4.00
Average Turnaround Time: 11.50

>> Round Robin (Quantum=3) Strategy:
Time 0: Running Job1 for 3 units
Time 3: Running Job2 for 3 units
...
Average Waiting Time: 4.50
Average Turnaround Time: 12.00

--- Memory Page Replacement Comparison ---
LRU Page Faults: 10
FIFO Page Faults: 9
```

## Platform Compatibility

✅ **Windows** (Primary)
- Visual Studio Code
- MinGW GCC Compiler
- Native Windows APIs for synchronization

✅ **Linux** (Original)
- GCC Compiler
- POSIX APIs (pthreads, semaphores)
- Native fork() for processes

## Notes

- The project is **pure C99** with no C++ features
- All STL containers replaced with C arrays/linked lists
- Cross-platform compatibility maintained throughout
- Memory management uses explicit malloc/free
- No external dependencies beyond standard C library

## Support

For build issues:
1. Check compiler path in VS Code settings
2. Verify all `.c` and `.h` files exist in expected locations
3. Run build task from terminal visible in VS Code
4. Check error messages in the output panel
