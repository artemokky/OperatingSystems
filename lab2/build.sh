#!/bin/bash

BUILD_DIR="build"

if [ -d "$BUILD_DIR" ]; then
    echo "Cleaning up build directory..."
    rm -rf "$BUILD_DIR"
fi

echo "Creating build directory..."
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

echo "Running CMake..."
cmake ..

echo "Building the project..."
make

if [ $? -ne 0 ]; then
    echo "Build failed"
    exit 1
else
    echo "Build succeeded"
fi

