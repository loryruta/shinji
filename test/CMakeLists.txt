include(${CMAKE_SOURCE_DIR}/shinji.cmake)

# ------------------------------------------------------------------------------------------------
# Executable
# ------------------------------------------------------------------------------------------------

add_executable(shinji_test
    "src/main.cpp"
)

shinji_validate_glsl(shinji_test "shaders/test.vert" OPTIONS "")
shinji_validate_glsl(shinji_test "shaders/test.frag" OPTIONS "")

shinji_embed(shinji_test "shinji_test"
    "shaders/test.vert"
    "shaders/test.frag"
)

shinji_compile_glsl_to_spirv(shinji_test "shaders/test.vert" "${CMAKE_CURRENT_SOURCE_DIR}/.spv/test.vert.spv" OPTIONS "--target-env=opengl")
shinji_compile_glsl_to_spirv(shinji_test "shaders/test.frag" "${CMAKE_CURRENT_SOURCE_DIR}/.spv/test.frag.spv" OPTIONS "--target-env=opengl")

shinji_embed(shinji_test "shinji_test"
    ".spv/test.vert.spv"
    ".spv/test.frag.spv"
)

shinji_finalize(shinji_test "shinji_test")

# ------------------------------------------------------------------------------------------------
# Dependencies
# ------------------------------------------------------------------------------------------------

# GLFW
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(shinji_test PUBLIC glfw)

# glad
find_package(glad CONFIG REQUIRED)
target_link_libraries(shinji_test PUBLIC glad::glad)
