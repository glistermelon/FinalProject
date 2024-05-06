#include "shaders.h"

#include <GL/glew.h>

#include <iostream>

unsigned int compile_shader(unsigned int type, std::string str, int* success) {
    unsigned int shader = glCreateShader(type);
    const char* c = str.c_str();
    glShaderSource(shader, 1, &c, nullptr);
    glCompileShader(shader);
    if (success) glGetShaderiv(shader, GL_COMPILE_STATUS, success);
    return shader;
}