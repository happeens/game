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
#include <text.hpp>

#define MAX_RECT_COUNT 1024

struct Viewport {
    glm::mat4 projection;
    u32 width = 0;
    u32 height = 0;

    GLuint ubo = 0;
};

struct ColoredRect {
    Position pos;
    Size size;
    Color color;

    ColoredRect(Position pos, Size size, Color color) :
        pos(pos), size(size), color(color) {};
};

struct TexturedRect {
    Position pos;
    Size size;
    SpriteId active_sprite;

    TexturedRect(Position pos, Size size, SpriteId active_sprite) :
        pos(pos), size(size), active_sprite(active_sprite) {};
};

struct CharacterRect {
    Position pos;
    f32 scale;
    char character;
    Color color;

    CharacterRect(Position pos, f32 scale, char character, Color color) :
        pos(pos), scale(scale), character(character), color(color) {};
};

using Rect = std::variant<ColoredRect, TexturedRect, CharacterRect>;

enum struct RenderGroupType {
    Primitive = 0,
    Sprite,
    Text,
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
    std::optional<std::shared_ptr<FontTexture>> font_texture;

    RenderGroup();
    ~RenderGroup();

    static std::shared_ptr<RenderGroup> primitive(
        Viewport viewport
    );
    static std::shared_ptr<RenderGroup> sprite(
        Viewport viewport,
        std::shared_ptr<Texture> texture
    );
    static std::shared_ptr<RenderGroup> text(
        Viewport viewport,
        std::shared_ptr<FontTexture> font_texture
    );

    void draw() const;

    void push_rect(ColoredRect rect);
    void push_rect(TexturedRect rect);
    void push_rect(CharacterRect rect);
};

