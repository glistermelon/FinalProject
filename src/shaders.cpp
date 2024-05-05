#include <shaders.h>

#include <GL/glew.h>
#include <fstream>
#include <sstream>

unsigned int compile_shader_file(unsigned int type, const std::string& file_name, int* success) {
    std::ifstream ifs(file_name);
    if (!ifs.is_open()) throw std::runtime_error("failed to open shader file: " + file_name);
    std::stringstream ss;
    ss << ifs.rdbuf();
    std::string s = ss.str();
    const char* c = s.c_str();
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &c, nullptr);
    glCompileShader(shader);
    if (success) glGetShaderiv(shader, GL_COMPILE_STATUS, success);
    return shader;
}