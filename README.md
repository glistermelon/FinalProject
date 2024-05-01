## Dependencies

* **GLFW**: https://www.glfw.org/ (build with CMake)
* **GLEW**: https://glew.sourceforge.net/
  * If on Linux, use Makefile to build
  * It comes with a "build" folder for anyone who wants to use CMake or Visual Studio
* **GLM**: https://github.com/g-truc/glm/releases/tag/1.0.1
  * *Build from source!* The "light" thing provided looks like its just headers and didn't work.

## CMakeLists WIP

This should work at the moment but is rather messy and unprofessional:

```cmake
cmake_minimum_required(VERSION 3.22.0)

project(physiks)

add_executable(
        physiks
        src/main.cpp
        src/Point.cpp
        src/Color.cpp
)

target_include_directories(physiks PRIVATE include)

set(GLEW_INCLUDE_DIR "/usr/local/include/glew/") # where the "GL" folder is (comes with glew)
set(GLEW_LIBRARY "/usr/local/lib/libglew.a") # glew library (need to compile glew)

find_package(OpenGL REQUIRED)
find_package (GLEW REQUIRED)
target_link_libraries(
        physiks
        OpenGL::GL
        GLEW::glew
        /usr/local/lib/libglfw3.a # glfw library
        /usr/local/lib/libglm.a  # opengl mathematics library
)
```