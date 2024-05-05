#pragma once

#include <string>

unsigned int compile_shader_file(unsigned int type, const std::string& file_name, int* success = nullptr);