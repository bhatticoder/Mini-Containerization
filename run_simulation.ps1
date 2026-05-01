# Build and run the simulation using g++
# 1. Create a directory for binaries
if (!(Test-Path bin)) { New-Item -ItemType Directory -Path bin }

# 2. Compile the project
# Note: We include the include directory, use C++17 for filesystem, and link necessary libraries
g++ -std=c++17 -I./include src/main.cpp -o bin/lite_container.exe

# 3. Check if compilation was successful and run
if ($?) {
    Write-Host "Build successful. Running simulation..." -ForegroundColor Green
    ./bin/lite_container.exe
} else {
    Write-Host "Build failed." -ForegroundColor Red
}
