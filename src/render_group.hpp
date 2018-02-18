#pragma once

#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <common.hpp>
#include <shader.hpp>
#include <texture.hpp>

#define MAX_RECT_COUNT 1024
#define VBO_RECT_ELEMS 8
#define EBO_RECT_ELEMS 6

struct Rect {
    f32 x_min;
    f32 x_max;
    f32 y_min;
    f32 y_max;
};

enum struct RenderGroupType {
    Primitive = 0,
    Sprite
};

struct RenderGroup {
    // TODO: this storage should be transient and
    // filled from game state every frame
    Rect rects[MAX_RECT_COUNT];
    u32 rect_count = 0;

    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;

    RenderGroupType type;

    std::shared_ptr<Shader> shader;
    std::shared_ptr<Texture> texture = nullptr;
    glm::mat4 projection;

    RenderGroup();
    ~RenderGroup();

    static std::shared_ptr<RenderGroup> primitive(glm::mat4 projection);
    static std::shared_ptr<RenderGroup> sprite(
        glm::mat4 projection,
        std::shared_ptr<Texture> texture
    );

    void draw() const;
    void push_rect(i32 x_min, i32 x_max, i32 y_min, i32 y_max);
};

