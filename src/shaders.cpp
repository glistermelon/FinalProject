#include "shaders.h"

#include <GL/glew.h>

#include <iostream>

unsigned int compile_shader(unsigned int type, std::string str, int* success) {
    std::cout << "--------------------" << std::endl;
    std::cout << str << std::endl;
    std::cout << "--------------------" << std::endl;
    unsigned int shader = glCreateShader(type);
    const char* c = str.c_str();
    glShaderSource(shader, 1, &c, nullptr);
    glCompileShader(shader);
    if (success) glGetShaderiv(shader, GL_COMPILE_STATUS, success);
    return shader;
}