cmake_minimum_required(VERSION 3.22.0)

project(physiks)

add_executable(
        physiks
        src/main.cpp
        src/Point.cpp
        src/Color.cpp
        src/Block.cpp
        src/shaders.cpp
)

target_include_directories(physiks PRIVATE include)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(GLM REQUIRED)
target_link_libraries(
        physiks
        OpenGL::GL
        GLEW::glew
        GLFW3::glfw
        GLM::glm
)

file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})