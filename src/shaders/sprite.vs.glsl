#version 330 core

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec2 tex_size;
layout (location = 2) in vec2 tex_offset;

out vec2 tex_position;

layout (std140) uniform Viewport {
    mat4 projection;
    uint viewport_width;
    uint viewport_height;
};

void main() {
    gl_Position = projection * vec4(
        a_position.x,
        viewport_height - a_position.y,
        0.0, 1.0
    );

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

    tex_position = tex_size * tex_position + tex_offset;
}

