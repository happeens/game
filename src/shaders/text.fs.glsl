#version 330 core

in vec2 tex_position;
in vec3 color;

out vec4 FragColor;

uniform sampler2D tex;

void main() {
    vec4 tex_color = texture(tex, tex_position);
    tex_color.r *= color.r;
    tex_color.g *= color.g;
    tex_color.b *= color.b;

    if (tex_color.a < 0.01) discard;

    FragColor = tex_color;
}
