#version 330 core

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec2 a_tex_position;

out vec2 tex_position;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(a_position, 0.0, 1.0);
    tex_position = a_tex_position;
}

