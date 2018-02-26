#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <common.hpp>

const static char ALLOWED_CHARACTERS[] =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

struct CharacterData {
};

struct FontTexture {
    GLuint id;

    u32 width = 50;
    u32 height = 50;

    FontTexture();
    ~FontTexture();

    void bind() const;
};

