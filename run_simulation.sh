#!/bin/bash
# Script to build and run the LiteContainer simulation

mkdir -p bin
g++ -I./include src/main.cpp -o bin/lite_container -lpthread

if [ $? -eq 0 ]; then
    echo "Build successful. Running simulation..."
    ./bin/lite_container
else
    echo "Build failed."
fi
