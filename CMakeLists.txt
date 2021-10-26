cmake_minimum_required(VERSION 3.19)
project(shader_injector)

set(CMAKE_CXX_STANDARD 20)

# VCPKG
include(FetchContent)
message(STATUS "Fetching vcpkg...")
FetchContent_Declare(vcpkg URL "https://github.com/microsoft/vcpkg/archive/refs/tags/2021.05.12.tar.gz")
FetchContent_MakeAvailable(vcpkg)
include("${vcpkg_SOURCE_DIR}/scripts/buildsystems/vcpkg.cmake")

add_subdirectory(test)
