@echo off
cd cmake_build
if exist CMakeCache.txt (
    echo Cache already exists.
) else (
    echo Generating cache.
    cmake ..
)
cmake --build .