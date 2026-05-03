@echo off
REM Build script for LiteContainer Engine on Windows
REM This script compiles all C source files and creates the executable

setlocal enabledelayedexpansion

echo.
echo ======================================
echo  LiteContainer Engine - Build Script
echo ======================================
echo.

REM Check if gcc is available
where gcc >nul 2>nul
if errorlevel 1 (
    echo ERROR: GCC not found!
    echo Please install MinGW and add it to your system PATH
    echo Download from: https://www.mingw-w64.org/
    pause
    exit /b 1
)

echo [*] Compiling LiteContainer Engine...
echo.

REM Create bin directory if it doesn't exist
if not exist "bin" mkdir bin

REM Compile all source files
gcc -I./include ^
    src/Process.c ^
    src/Scheduler.c ^
    src/FCFSScheduler.c ^
    src/RoundRobinScheduler.c ^
    src/MemoryManager.c ^
    src/FIFOMemoryManager.c ^
    src/ContainerManager.c ^
    src/ResourceSync.c ^
    src/IPCManager.c ^
    src/BankersAlgorithm.c ^
    src/main.c ^
    -o bin/lite_container.exe ^
    -Wall -std=c99

REM Check if compilation was successful
if errorlevel 1 (
    echo.
    echo ERROR: Compilation failed!
    pause
    exit /b 1
)

echo.
echo [+] Build successful!
echo [+] Executable created: bin\lite_container.exe
echo.

REM Ask if user wants to run the program
set /p run_now="Do you want to run the program now? (y/n): "
if /i "%run_now%"=="y" (
    echo.
    echo [*] Starting LiteContainer Engine...
    echo.
    bin\lite_container.exe
)

pause
exit /b 0
