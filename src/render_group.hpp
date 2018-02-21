#pragma once

#include <memory>
#include <variant>
#include <optional>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <common.hpp>
#include <shader.hpp>
#include <texture.hpp>

#define MAX_RECT_COUNT 1024

struct ColoredRect {
    f32 x_min;
    f32 x_max;
    f32 y_min;
    f32 y_max;
};

struct TexturedRect {
    f32 x_min;
    f32 x_max;
    f32 y_min;
    f32 y_max;
};

using Rect = std::variant<ColoredRect, TexturedRect>;

enum struct RenderGroupType {
    Primitive = 0,
    Sprite
};

struct RenderGroup {
    // TODO: this storage should be transient and
    // filled from game state every frame
    std::vector<Rect> rects = {};

    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;

    RenderGroupType type;

    std::shared_ptr<Shader> shader;
    std::optional<std::shared_ptr<Texture>> texture;
    glm::mat4 projection;

    RenderGroup();
    ~RenderGroup();

    static std::shared_ptr<RenderGroup> primitive(glm::mat4 projection);
    static std::shared_ptr<RenderGroup> sprite(
        glm::mat4 projection,
        std::shared_ptr<Texture> texture
    );

    void draw() const;

    void push_rect(ColoredRect rect);
    void push_rect(TexturedRect rect);
};

