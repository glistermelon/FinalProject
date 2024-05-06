cmake_minimum_required(VERSION 3.22.0)

project(physiks)

add_executable(
        physiks
        src/main.cpp
        src/Point.cpp
        src/Color.cpp
        src/Block.cpp
)

target_include_directories(physiks PRIVATE include)

add_compile_definitions(REPLIT)