include(${CMAKE_SOURCE_DIR}/shinji.cmake)

add_executable(shinji_test src/main.cpp)

shinji_load(shinji_test)

# ------------------------------------------------------------------------------------------------
# Dependencies
# ------------------------------------------------------------------------------------------------

# GLFW
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(shinji_test PUBLIC glfw)

# glad
find_package(glad CONFIG REQUIRED)
target_link_libraries(shinji_test PUBLIC glad::glad)
