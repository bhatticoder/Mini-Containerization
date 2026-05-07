# B2B Marketing Campaign Management System

A simulation of a Business-to-Business marketing platform designed to demonstrate real operating system concepts in a practical domain. This project maps marketing campaigns to process execution, resource allocation, scheduling, memory management, synchronization, IPC, and deadlock avoidance.

## Introduction

This project models a B2B marketing engine where multiple agencies launch and manage campaigns concurrently. Each campaign consumes budget and team capacity, competes for shared marketing resources, and requires coordination across the system. The goal is to show how operating system algorithms solve real-world problems in a simulated business environment.

## How We Solved It (OS Concepts)

### Process Management
- Agencies and campaigns are represented as independent processes.
- Each campaign follows a lifecycle: planning, execution, analysis, and completion.
- Processes can be in states such as `READY`, `RUNNING`, `WAITING`, and `TERMINATED`.
- The `Process` module abstracts campaign tasks with attributes like arrival time, burst time, and priority.

### CPU Scheduling
- **FCFS (First Come First Served)** handles campaigns in arrival order, simulating sequential project execution.
- **Round Robin** uses a fixed time quantum to share CPU time between campaigns fairly.
- Priority values allow urgent campaigns to be scheduled with higher importance.
- The scheduler module compares FCFS and Round Robin performance metrics such as waiting and turnaround time.

### Memory Management
- The simulation includes a cache for campaign analytics and creative assets.
- **LRU (Least Recently Used)** preserves frequently accessed campaign data in cache.
- **FIFO (First In First Out)** models simpler cache eviction behavior.
- Page fault counting demonstrates the cost of cache misses in campaign analytics processing.

### Resource Synchronization
- Shared marketing resources such as budget pools and analytics tools require safe access.
- Mutex locks protect shared file write operations between campaigns.
- Semaphores control access to limited devices or resources, ensuring team capacity is not overused.
- The `ResourceSync` module shows how concurrent campaign operations stay consistent.

### Inter-Process Communication (IPC)
- The system simulates communication between agencies through shared state and task files.
- Campaign analytics and coordination data are exchanged as shared resources.
- IPC concepts are represented by the management of campaign task metadata and collaborative operations.

### Deadlock Prevention
- The Banker's Algorithm ensures campaign resource allocation remains in a safe state.
- The system checks whether granting a new budget/capacity request could lead to deadlock.
- Unsafe allocations are prevented, modeling how operating systems maintain overall system stability.

## How the Project Works

### System Architecture
- `src/main.c`: CLI interface and simulation driver.
- `src/ContainerManager.c`: Manages campaign lifecycle, resource tracking, and persistence.
- `src/Process.c`: Defines process/campaign attributes and state management.
- `src/FCFSScheduler.c` / `src/RoundRobinScheduler.c`: Implement CPU scheduling algorithms.
- `src/MemoryManager.c` / `src/FIFOMemoryManager.c`: Implement cache replacement policies.
- `src/ResourceSync.c`: Implements mutex and semaphore synchronization examples.
- `src/BankersAlgorithm.c`: Implements deadlock safety checking.

### User Workflow
1. Start the program using the compiled executable or `run_simulation.ps1`.
2. Configure global marketing resources with `config <budget> <capacity>`.
3. Launch campaign processes with `launch <agency> <campaign> [-b budget] [-c capacity]`.
4. View active campaigns and resource usage using `campaigns`.
5. Pause or stop a campaign using `pause <agency>`.
6. Run `analytics` to compare scheduling, memory, synchronization, and deadlock prevention behavior.

### Example Scenario
- `config 100 20` sets a global budget of $100K and 20 capacity units.
- `launch AgencyX CampaignA -b 40 -c 10` starts a campaign consuming budget and team capacity.
- `campaigns` shows active campaigns and current resource usage.
- `analytics` prints performance reports for scheduling, cache management, and safety.

## Project Structure
- `include/`: Header files for OS modules.
- `src/`: Core source files implementing the marketing simulation.
- `campaigns/`: Stores campaign workspaces and persistent state.
- `bin/`: Compiled executable output.
- `run_simulation.ps1`: Build and run helper script for Windows.

## How to Run
1. Install GCC (MinGW) and ensure it is in your PATH.
2. Open the project folder in VS Code or a terminal.
3. Build the project:
   ```powershell
   gcc -I./include src/*.c -o bin/lite_container.exe -Wall -std=c99
   ```
4. Run the executable:
   ```powershell
   .\bin\lite_container.exe
   ```

## Commands
- `config <budget> <capacity>`: Set available marketing resources.
- `launch <agency> <campaign> [-b budget] [-c capacity]`: Start a campaign.
- `campaigns`: List active campaigns and resource usage.
- `pause <agency>`: Pause a campaign and free resources.
- `analytics`: Show scheduling, memory, sync, and deadlock reports.
- `sync`: Demonstrate shared resource synchronization.
- `help`: Show available commands.
- `exit`: Exit the program.
