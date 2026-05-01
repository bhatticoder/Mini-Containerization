# LiteContainer Engine

A lightweight containerization simulation engine inspired by Docker, built for Operating Systems (OS) concepts demonstration.

## Features
- **Process Management**: Simulated container lifecycle using `fork()` (Linux) and process abstraction (Windows).
- **Docker-like CLI**: Commands like `run`, `ps`, `stop`, `config`, and `stats`.
- **Isolation**: Each container runs in its own logical workspace (directory-based isolation).
- **Resource Management**: Global RAM and Disk configuration with per-container quotas.
- **CPU Scheduling**: Comparison between **FCFS** and **Round Robin** algorithms.
- **Memory Management**: Comparison between **LRU** and **FIFO** page replacement policies.
- **Persistence**: Engine state and container task data are stored in JSON/DAT files.

## Project Structure
- `include/`: Header files for OS modules (Schedulers, Memory Managers, etc.).
- `src/`: Core engine logic (`main.cpp`).
- `containers/`: Default directory for isolated container workspaces and persistent state.
- `run_simulation.ps1`: Windows PowerShell script to build and run the project.

## How to Run
1. Ensure you have `g++` (MinGW) installed and in your PATH.
2. Run the simulation:
   ```powershell
   .\run_simulation.ps1
   ```

## CLI Commands
- `config <ram> <disk>`: Set system resource limits.
- `run <name> <task> [-m mem] [-d disk]`: Launch a container.
- `ps`: View running/exited containers and system load.
- `stop <name>`: Terminate a container and release resources.
- `stats`: Generate algorithm performance comparison reports.
