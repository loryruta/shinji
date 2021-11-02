cmake_minimum_required(VERSION 3.19)
project(shader_injector)

set(CMAKE_CXX_STANDARD 17)

option(SHINJI_BUILD_TESTS "Build tests?" OFF)

# VCPKG
include(FetchContent)
message(STATUS "Fetching vcpkg...")
FetchContent_Declare(vcpkg URL "https://github.com/microsoft/vcpkg/archive/refs/tags/2021.05.12.tar.gz")
FetchContent_MakeAvailable(vcpkg)
include("${vcpkg_SOURCE_DIR}/scripts/buildsystems/vcpkg.cmake")

if (SHINJI_BUILD_TESTS)
    add_subdirectory(test)
endif()
