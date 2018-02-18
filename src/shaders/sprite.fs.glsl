#version 330 core

in vec2 tex_position;

out vec4 FragColor;

uniform sampler2D tex;

void main() {
    FragColor = texture(tex, tex_position);
}

