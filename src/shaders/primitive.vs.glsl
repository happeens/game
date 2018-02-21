#version 330 core

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec3 a_color;

out vec3 color;

layout (std140) uniform Viewport {
    mat4 projection;
    uint viewport_width;
    uint viewport_height;
};

void main() {
    gl_Position = projection * vec4(
        a_position.x
        a_position.y,
        0.0, 1.0
    );

    color = a_color;
}

