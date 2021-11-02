

# SHader INJ(I)ector

SHINJI (originally SHader INJector) is a CMake addon that aims to avoid CMake boilerplate code for resource management and exposes simple and easy to use functions.

It can:
- Validate GLSL code (using glslangValidator)
- Compile GLSL code to SPIR-V (using glslc)
- Embed resources in the application binary (internally using [CMRC](https://github.com/vector-of-bool/cmrc)'s code)

## Requirements:
* CMake 3.19
* C++17
* [Vulkan SDK](https://vulkan.lunarg.com) (required only if using shader-related functions)

## Usage

Using SHINJI is pretty straight-forward as you can dynamically download and depend on it in configuration time:

```cmake
include(FetchContent)
message(STATUS "Fetching shinji...")
FetchContent_Declare(
    shinji
    GIT_REPOSITORY "https://github.com/loryruta/shinji"
)
FetchContent_MakeAvailable(shinji)
include("${shinji_SOURCE_DIR}/shinji.cmake")
```

You may now want to use the following functions:

```cmake
shinji_validate_glsl(<TARGET> <SHADER> OPTIONS <GLSLANG_VALIDATOR_OPTIONS>)
# shinji_validate_glsl(shinji_test "shaders/my_shader.vert")

shinji_compile_glsl_to_spirv(<TARGET> <GLSL_SHADER> <SPIRV_SHADER> OPTIONS <GLSLC_OPTIONS>)
# shinji_compile_glsl_to_spirv(shinji_test "shaders/my_shader.vert" ".spv/my_shader.vert.spv")

shinji_embed(<TARGET> <FILE1> [<FILE2> ...])
# shinji_embed(shinji_test "shaders/my_shader.vert" "shaders/my_shader.frag")

# IMPORTANT:
# THIS LINE MUST BE PRESENT AFTER *ALL* shinji_* CALLS!
shinji_finalize(<TARGET>)
# shinji_finalize(<TARGET>)
```

GLSL shader validation and compilation, and resource integration will take place in build time.

In order to access the resources (embedded or not):
```c++
#include <shinji.hpp>

void something()
{
    // Bundled resource:
    auto [buf, buf_len] = shinji::load_resource_from_bundle("shaders/my_shader.vert");
    // (...)
    
    // Physical file resource:
    std::vector<char> buf;
    shinji::load_resource_from_file("shaders/my_shader.vert", buf);
    // (...)
}

```



