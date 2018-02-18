#version 330 core

layout (location = 0) in vec2 a_position;

out vec2 tex_position;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(a_position, 0.0, 1.0);

    int rect_corner = gl_VertexID % 4;

    if (rect_corner == 0) {
        tex_position = vec2(0.0, 1.0);
    } else if (rect_corner == 1) {
        tex_position = vec2(1.0, 1.0);
    } else if (rect_corner == 2) {
        tex_position = vec2(0.0, 0.0);
    } else {
        tex_position = vec2(1.0, 0.0);
    }
}

