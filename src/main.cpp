#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

#include <Block.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int main() {

    const int INITIAL_WINDOW_WIDTH = 1920;
    const int INITIAL_WINDOW_HEIGHT = 1080;

    if (!glfwInit()) throw std::runtime_error("glfw init failed");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 5); // Anti-aliasing!

    auto* window = glfwCreateWindow(
            INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, "[window title]",
            nullptr, nullptr
    );
    if (!window) throw std::runtime_error("window creation failed");

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) throw std::runtime_error("glew init failed");

    Block::init_static_render_cache(); // call once when app starts

    // Example of rendering a yellow square
    // Anyone can delete this if you want to
    Block b;
    b.set_position(100, 100);
    b.add_vertex(Point(0, 0));
    b.add_vertex(Point(200, 0));
    b.add_vertex(Point(200, 200));
    b.add_vertex(Point(0, 200));
    b.update_render_cache(); // must be called after vertices are modified
    b.set_color(Color::YELLOW); // or Color(255, 255, 0)

    while (!glfwWindowShouldClose(window)) {
        glUseProgram(Block::get_gl_program());
        b.render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

}