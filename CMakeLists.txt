cmake_minimum_required(VERSION 3.22.0)

project(physiks)

add_executable(
        physiks
        src/main.cpp
        src/Point.cpp
        src/Color.cpp
        src/Block.cpp
        src/shaders.cpp
        src/Quadtree.cpp
        src/Vect2.cpp
        src/Triangle.cpp
        src/Screen.cpp
)

target_include_directories(physiks PRIVATE include)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

if (EXISTS ${PROJECT_SOURCE_DIR}/deps)
        list(APPEND CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/deps)
endif()

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

# inspired by, but not taken from https://gist.github.com/sivachandran/3a0de157dccef822a230
file(GLOB_RECURSE shader_files "${PROJECT_SOURCE_DIR}/shaders/*.glsl")

set(shader_header_filename "cmake_gen_shaders.hpp")
set(shader_source_filename "cmake_gen_shaders.cpp")
string(APPEND shader_header "#pragma once\n#include<string>\n")
string(APPEND shader_source "#include \"${shader_header_filename}\"")

foreach(file ${shader_files})

    file(READ ${file} shader_str HEX)
    string(REPLACE "${PROJECT_SOURCE_DIR}/" "" file ${file})
    string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," shader_str ${shader_str})
    set(shader_str std::string({${shader_str}}))

    string(REPLACE ".glsl" "" var ${file})
    string(REPLACE "/" "_" var ${var})
    string(TOUPPER ${var} var)

    message(${var})

    string(APPEND shader_header "\nextern const std::string ${var}\\;")
    string(APPEND shader_source "\nconst std::string ${var} = ${shader_str}\\;")

endforeach()

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/${shader_header_filename}" ${shader_header})
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/${shader_source_filename}" ${shader_source})
target_sources(physiks PRIVATE ${shader_source_filename})
target_include_directories(physiks PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")

add_compile_definitions(GLEW_STATIC)

# Tell Visual Studio to run the proper executable
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT physiks)