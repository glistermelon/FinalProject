#ifndef REPLIT
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#endif

#include <stdexcept>

#include "../include/Block.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int main() {

#ifndef REPLIT
    
    const int INITIAL_WINDOW_WIDTH = 1920;
    const int INITIAL_WINDOW_HEIGHT = 1080;

    if (!glfwInit()) throw std::runtime_error("glfw init failed");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 5); // Anti-aliasing!

    auto* window = glfwCreateWindow(
            INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, "Physiks Simulator",
            nullptr, nullptr
    );
    if (!window) throw std::runtime_error("window creation failed");

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) throw std::runtime_error("glew init failed");

    Block::init_static_render_cache(); // call once when app starts

#endif

    // Example of rendering a yellow square
    // Anyone can delete this if you want to
    Block b {100, 200, 100, 200, 1};

#ifndef REPLIT
    b.update_render_cache(); // must be called after vertices are modified
#endif
    
    b.set_color(Color::YELLOW); // or Color(255, 255, 0)

#ifndef REPLIT
    
    while (!glfwWindowShouldClose(window)) {
        glUseProgram(Block::get_gl_program());
        b.render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

#endif

}