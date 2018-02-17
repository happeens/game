#version 330 core

in vec2 tex_position;

out vec4 out_color;

uniform sampler2D texture;

void main() {
    out_color = texture(texture, tex_position);
}

