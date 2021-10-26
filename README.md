

# SHader INJ(I)ector

SHINJI is a CMake addon that helps you managing GLSL shaders validation, compilation and deployment.

### Overview

_Because of its architecture, **SHINJI only acts in project configuration time**._

As mentioned SHINJI divides the shader processing in 3 parts:
* **Validation**: validates GLSL shaders using the Vulkan SDK tool `glslanValidator`.
* **Compilation**: offers you the possibility to compile GLSL shaders to SPIR-V using the Vulkan SDK tool `glslc`.
* **Deployment**: I'm aware "deployment" isn't a very used word in this area: for "deployment" I mean the process of making the shaders available to the built application. In a common scenario this translates to copying the shaders from the source to the build folder. Other than this, **SHINJI can embed shaders in the application binary (really useful for libraries!)**.

### Requirements

The following are the minimum requirements needed to run SHINJI:
* CMake 3.19
* C++17 
* Python3
* Vulkan SDK

## Usage

You have to describe how SHINJI will treat your shaders through a configuration file: `shinji.json`.

```javascript
{
    "generated_file": "src/generated/shinji.hpp",
    "embed": {
         "shaders": [
            "shaders/test_txt.vert",
            {                                          
                "type": "glslc",
                "name": "shaders/test_spv.vert.spv",
                "input": "shaders/test_spv.vert",
                "glslc_options": "--target-env=opengl"
            }
        ]
    }
}
```
The code above describes two shaders: `shaders/test_txt.vert` (textual) and `shaders/test_spv.vert.spv` (compiled from `shaders/test_spv.vert`) that needs to be embedded in the application.

**The field `generated_file` is required** and specify an auto-generated file holding, in this case, the shaders and shader loading functions.

- **NOTE: `shinji.json` must always lay in the project root folder (where `CMakeLists.txt` is).**
- NOTE: relative paths are treated relative to `CMakeLists.txt`.

In order to load SHINJI from `CMakeLists.txt`:
```cmake
# todo download shinji package
include(${CMAKE_SOURCE_DIR}/shinji.cmake)

add_executable(test_app ...)  
  
shinji_load(test_app)
```

To retrieve the shader source in code you can use the following functions:
```c++
#include "src/generated/shinji.hpp" // Or whatever you've wrote in `shinji.json`.

{
    auto [src, src_len] = shinji::load_shader_from_bundle("shaders/test_txt.vert");
    // Use OpenGL/Vulkan or whatever to load the shader...
}

{
    auto [src, src_len] = shinji::load_shader_from_bundle("shaders/test_spv.vert.spv");
    // ...
}
```




