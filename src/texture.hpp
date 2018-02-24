#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <initializer_list>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <common.hpp>

struct SpriteData {
    Position tex_pos;
    Size tex_size;

    SpriteData(Position tex_pos, Size tex_size) :
        tex_pos(tex_pos), tex_size(tex_size) {};
};

struct BatchInput {
    BatchInput(const std::initializer_list<std::string>& input) :
        values(input) {};

    const std::vector<std::string> values;
};

struct Texture {
    GLuint id;

    u32 width = 0;
    u32 height = 0;
    u32 channels = 0;

    std::unordered_map<std::string, SpriteId> sprite_ids = {};
    std::vector<SpriteData> sprites = {};

    Texture(const std::string& path);
    Texture(const std::vector<std::string>& batch);
    ~Texture();

    void bind() const;
};

