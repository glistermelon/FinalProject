// basic fragment shader that just sets a uniform color for every vertex

#version 460 core

uniform vec4 color;
out vec4 out_color;

void main() {
    out_color = color;
}