#pragma once

#include <stdio.h>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <shader.hpp>
#include <types.hpp>

#define MAX_RECT_COUNT 1024
#define VBO_RECT_ELEMS 8
#define EBO_RECT_ELEMS 6

struct Rect {
    f32 x_min;
    f32 x_max;
    f32 y_min;
    f32 y_max;
};

struct RenderGroup {
    Rect rects[MAX_RECT_COUNT];
    u32 rect_count;

    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    std::shared_ptr<Shader> shader;
    glm::mat4 projection;

    RenderGroup(std::shared_ptr<Shader> shader, glm::mat4 projection);
    ~RenderGroup();

    void draw() const;
};

