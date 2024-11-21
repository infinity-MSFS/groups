@echo off
@REM cd cmake_build
@REM if exist CMakeCache.txt (
@REM     echo Cache already exists.
@REM     cmake ..
@REM
@REM ) else (
@REM     echo Generating cache.
@REM     cmake ..
@REM )

cmake -DCMAKE_BUILD_TYPE=Release -S "." -B "cmake-build-release"
cmake --build "cmake-build-release" --target Runner