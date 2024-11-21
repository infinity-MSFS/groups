#cmake_minimum_required(VERSION 3.29)
#project(Runner)
#add_executable(Runner main.cpp)

cmake_minimum_required(VERSION 3.29)

include(FetchContent)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(VCPKG_SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg-debug")
    message("${Blue}Creating vcpkg at: ${Green}${VCPKG_SOURCE_PATH}${ColorReset}")
else ()
    set(VCPKG_SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg-release")
    message("${Blue}Creating vcpkg at: ${Green}${VCPKG_SOURCE_PATH}${ColorReset}")
endif ()

set(ENV{VCPKG_ENABLE_MANIFEST_MODE} "OFF")
set(ENV{VCPKG_ROOT} "${VCPKG_SOURCE_PATH}")
if (NOT EXISTS "${VCPKG_SOURCE_PATH}")

    FetchContent_Declare(
            vcpkg
            GIT_REPOSITORY https://github.com/microsoft/vcpkg.git
            GIT_TAG 2024.10.21
            SOURCE_DIR ${VCPKG_SOURCE_PATH}
    )
    FetchContent_MakeAvailable(vcpkg)

    if (WIN32)
        set(BOOTSTRAP_COMMAND "${VCPKG_SOURCE_PATH}/bootstrap-vcpkg.bat")

    else ()
        set(BOOTSTRAP_COMMAND "${VCPKG_SOURCE_PATH}/bootstrap-vcpkg.sh")

    endif ()

    if (WIN32)
        if (NOT EXISTS "${VCPKG_SOURCE_PATH}/vcpkg.exe")
            message("${Green}Bootstrapping vcpkg...${ColorReset}")
            execute_process(
                    COMMAND ${BOOTSTRAP_COMMAND}
                    WORKING_DIRECTORY ${VCPKG_SOURCE_PATH}
            )
        endif ()
    else ()
        if (NOT EXISTS "${VCPKG_SOURCE_PATH}/vcpkg")
            message("${Green}Bootstrapping vcpkg...${ColorReset}")
            execute_process(
                    COMMAND ${BOOTSTRAP_COMMAND}
                    WORKING_DIRECTORY ${VCPKG_SOURCE_PATH}
            )
        endif ()
    endif ()

endif ()

if (WIN32)
    set(VCPKG_TRIPLET "x64-windows-static")
else ()
    set(VCPKG_TRIPLET "x64-linux-static")
endif ()

message("${Blue}Installing Zlib${ColorReset}${Green}")
execute_process(
        COMMAND ${VCPKG_SOURCE_PATH}/vcpkg install zlib
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

message("${Blue}Installing msgpack${ColorReset}${Green}")
execute_process(
        COMMAND ${VCPKG_SOURCE_PATH}/vcpkg install msgpack
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

message("${Blue}Installing OpenSSL${ColorReset}${Green}")
execute_process(
        COMMAND ${VCPKG_SOURCE_PATH}/vcpkg install openssl
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

message("${Blue}Installing Curl${ColorReset}${Green}")
execute_process(
        COMMAND ${VCPKG_SOURCE_PATH}/vcpkg install curl
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

message("${Blue}Installing MiniZip${ColorReset}${Green}")
execute_process(
        COMMAND ${VCPKG_SOURCE_PATH}/vcpkg install minizip
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

message("${Blue}Installing Zoe${ColorReset}${Green}")
execute_process(
        COMMAND ${VCPKG_SOURCE_PATH}/vcpkg install zoe
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

set(CMAKE_TOOLCHAIN_FILE "${VCPKG_SOURCE_PATH}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")

project(Runner)

set(CMAKE_CXX_STANDARD 23)

message("${Blue}Using toolchain file: ${Green}${CMAKE_TOOLCHAIN_FILE}")
find_package(ZLIB REQUIRED)
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(msgpack-cxx CONFIG REQUIRED)
find_package(unofficial-minizip CONFIG REQUIRED)
find_package(zoe CONFIG REQUIRED PATHS "${VCPKG_SOURCE_PATH}/packages/zoe_${VCPKG_TRIPLET}/")

#print_system_dependencies_info()

message(STATUS "${BoldMagenta}Checking build Environment: ${ColorReset} ")

message("${Blue}Project: ${Green}${CMAKE_PROJECT_NAME}${ColorReset}")
message("${Blue}Release Profile: ${Green}${CMAKE_BUILD_TYPE}${ColorReset}")
message("${Blue}Compiler: ${Green}${CMAKE_CXX_COMPILER_ID}${ColorReset}")
message("${Blue}Compiler Version: ${Green}${CMAKE_CXX_COMPILER_VERSION}${ColorReset}")
message("${Blue}Fetching Remote Dependencies${Green}")

message(STATUS "${BoldMagenta}Environment configuration complete")
message("${Cyan}")

message("${Blue}Gathering Infinity Launcher Source Files")
add_executable(Runner
        # -- Main Source File --
        main.cpp
        src/Deserialize.hpp
)

message("${Blue}Setting Link Paths")
if (UNIX)
    target_include_directories(Runner PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
            ${ZOE_INCLUDE_DIR}
            ${MINIZIP_INCLUDE_DIR}
            ${DBUS_INCLUDE_DIRS}
    )
else ()
    target_include_directories(Runner PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
            ${infinity_SOURCE_DIR}/include
            ${infinity_SOURCE_DIR}/src/imgui
            ${ZOE_INCLUDE_DIR}
            ${MINIZIP_INCLUDE_DIR}
    )
endif ()

target_compile_definitions(Runner PRIVATE
        $<$<CONFIG:Release>:RELEASE_DIST>
)

if (WIN32)
    message("${Blue}Setting Up Windows Console Configuration")
    target_link_options(Runner PRIVATE
            $<$<CONFIG:Release>:/SUBSYSTEM:WINDOWS>
            $<$<NOT:$<CONFIG:Release>>:/SUBSYSTEM:CONSOLE>
    )
    message("${Blue}Setting Up Zoe Dynamic Link")
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(ZOE_LIB ${VCPKG_SOURCE_PATH}/packages/zoe_x64-windows/debug/lib/libZoe-d.lib)
        add_custom_command(TARGET Runner POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${VCPKG_SOURCE_PATH}/packages/zoe_x64-windows/debug/bin/libZoe-d.dll
                $<TARGET_FILE_DIR:Runner>
        )
    else ()
        set(ZOE_LIB ${VCPKG_SOURCE_PATH}/packages/zoe_x64-windows/lib/libZoe.lib)
        add_custom_command(TARGET Runner POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${VCPKG_SOURCE_PATH}/packages/zoe_x64-windows/bin/libZoe.dll
                $<TARGET_FILE_DIR:Runner>
        )
    endif ()

    message("${Blue}Setting Up OpenSSL Dynamic Link")
    add_custom_command(TARGET Runner POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${infinity_SOURCE_DIR}/bin/libcrypto-3-x64.dll
            $<TARGET_FILE_DIR:Runner>
    )
else ()

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(ZOE_LIB ${VCPKG_SOURCE_PATH}/packages/zoe_x64-linux/debug/lib/liblibZoe-d.a)
    else ()
        set(ZOE_LIB ${VCPKG_SOURCE_PATH}/packages/zoe_x64-linux/lib/liblibZoe.a)
    endif ()
endif ()
message("${Blue}Configuring Linker")
target_link_libraries(Runner PRIVATE CURL::libcurl OpenSSL::SSL OpenSSL::Crypto unofficial::minizip::minizip ZLIB::ZLIB ${ZOE_LIB} msgpack-cxx ${TOAST_LIB})