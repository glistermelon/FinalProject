#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

int main() {

    if (!glfwInit()) throw std::runtime_error("glfw init failed");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto* window = glfwCreateWindow(
            1920, 1080, "[window title]",
            nullptr, nullptr
    );
    if (!window) throw std::runtime_error("window creation failed");

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) throw std::runtime_error("glew init failed");

    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

}