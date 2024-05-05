## Building

### Dependencies

* **GLFW**: https://www.glfw.org/ (build with CMake)
* **GLEW**: https://glew.sourceforge.net/
  * If on Linux, use Makefile to build
  * It comes with a "build" folder for anyone who wants to use CMake or Visual Studio
* **GLM**: https://github.com/g-truc/glm/releases/tag/1.0.1
  * *Build from source!* The "light" thing provided looks like its just headers and didn't work.

### Setting imported library variables without the CMake GUI

Particularly to import GLFW and GLM, you'll probably have to define
`GLFW3_INCLUDE_DIR` amd `GLM_INCLUDE_DIR`, and maybe `GLFW3_LIBRARY`
and `GLM_LIBRARY`. You can do this with `set()` in `CMakeLists.txt`,
before `find_package()` is called.

Here's an example of setting `GLFW3_INCLUDE_DIR` and `GLFW3_LIBRARY`
on Windows, where `C:/folder/include/glfw3` contains glfw3.h.
```CMake
set(GLFW3_INCLUDE_DIR C:/folder/include/glfw3)
set(GLFW3_LIBRARY C:/folder/lib/glfw3.lib)
```