#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <common.hpp>

struct CharacterData {
    u32 width;
    u32 height;

    u32 pos_x;
    u32 pos_y;
};

struct FontTexture {
    GLuint id;

    u32 width;
    u32 height;

    std::unordered_map<char, CharacterData> characters = {};

    FontTexture();
    ~FontTexture();

    void bind() const;
};

