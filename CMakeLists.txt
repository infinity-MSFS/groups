cmake_minimum_required(VERSION 3.29)

include(FetchContent)
include(cmake/env.cmake)
include(cmake/colors.cmake)

set(VCPKG_ROOT $ENV{VCPKG_ROOT})

if (VCPKG_ROOT)
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
    message("${Blue}Using Toolchain file: ${Green}${CMAKE_TOOLCHAIN_FILE}${ColorReset}")
else ()
    message(FATAL_ERROR "VCPKG_ROOT is not set in the environment!")
endif ()

project(Runner)
set(CMAKE_CXX_STANDARD 23)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(INFINITY_TESTS)
endif ()

find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)
find_package(msgpack-cxx CONFIG REQUIRED)

message(STATUS "${BoldMagenta}Checking build Environment: ${ColorReset} ")

message("${Blue}Project: ${Green}${CMAKE_PROJECT_NAME}${ColorReset}")
message("${Blue}Release Profile: ${Green}${CMAKE_BUILD_TYPE}${ColorReset}")
message("${Blue}Compiler: ${Green}${CMAKE_CXX_COMPILER_ID}${ColorReset}")
message("${Blue}Compiler Version: ${Green}${CMAKE_CXX_COMPILER_VERSION}${ColorReset}")

load_encryption_key_from_env_file()

message("${Blue}Fetching Remote Dependencies${Green}")

message(STATUS "${BoldMagenta}Environment configuration complete")
message("${Cyan}")

message("${Blue}Gathering Infinity Launcher Source Files")
add_executable(Runner
        # -- Main Source File --
        src/main.cpp
        src/Encryption/Encryption.cpp
        src/Encryption/Encryption.hpp
        src/Builder/Builder.cpp
        src/Builder/Builder.hpp
        src/Deserialize/Deserialize.cpp
        src/Deserialize/Deserialize.hpp
        src/Fetcher/Fetcher.cpp
        src/Fetcher/Fetcher.hpp
        src/BinaryStructure/BinaryStructure.hpp
)

message("${Blue}Setting Link Paths")

target_include_directories(Runner PRIVATE
        src/
        ${CMAKE_SOURCE_DIR}/vcpkg_installed/include
)

message("${Blue}Configuring Linker")
target_link_libraries(Runner PRIVATE CURL::libcurl OpenSSL::SSL OpenSSL::Crypto ZLIB::ZLIB msgpack-cxx)