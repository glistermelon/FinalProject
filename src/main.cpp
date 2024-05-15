#ifndef REPLIT
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

#include <iostream>
#include <stdexcept>
#include <chrono>

#include "../include/Screen.h"
#include "../include/Block.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// For all the blocks
#include <vector>
std::vector<Block*> blocks;

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
    std::cout << "How many non-static blocks do you want?" << std::endl;
    int n;
    std::cin >> n;
    std::cout << "Please input: centerX centerY width height velocityX velocityY mass" << std::endl;
    for (int i = 0; i < n; i++) {
        double cX,cY,w,h,vX,vY,m;
        std::cin >> cX >> cY >> w >> h >> vX >> vY >> m;
        blocks.push_back(new Block(cX, cY, w, h, vX, vY, m));
    }
    std::cout << "How many static blocks do you want?" << std::endl;
    std::cin >> n;
    std::cout << "Please input: centerX centerY width height velocityX velocityY mass" << std::endl;
    for (int i = 0; i < n; i++) {
        double cX,cY,w,h,vX,vY,m;
        std::cin >> cX >> cY >> w >> h >> vX >> vY >> m;
        blocks.push_back(new Block(cX, cY, w, h, vX, vY, m));
        blocks[blocks.size() - 1]->is_static = true;
    }

    // Ground
    Block b3 {500, 0, 2000, 10, 0, 0, 1};
    b3.is_static = true;
    blocks.push_back(&b3);

#ifndef REPLIT
    for (auto* block : blocks) block->update_render_cache();  // must be called after vertices are modified
    for (auto* block : blocks) block->set_color(Color::WHITE);
#endif
    
#ifndef REPLIT

    const unsigned int fps = 30;
    
    Screen screen;
    screen.blocks = blocks;

    for (Block* block : blocks) screen.head.right->insert_block(screen.head.right, block);

    double interval_between_frames = 1.0 / fps;
    auto startTime = std::chrono::high_resolution_clock::now(), currTime = startTime, prevTime = startTime;

    int frameCounter = 0;

    while (!glfwWindowShouldClose(window)) {
        glUseProgram(Block::get_gl_program());
        currTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = currTime - prevTime;
        double durationInSeconds = static_cast<double>(duration.count());
        if (durationInSeconds > interval_between_frames) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            screen.update();
            prevTime = currTime;
            glfwSwapBuffers(window);
        }
        glfwPollEvents();
    }

#endif

}