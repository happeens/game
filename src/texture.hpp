#pragma once

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <types.hpp>

struct Texture {
    GLuint id;

    i32 width = 0;
    i32 height = 0;
    i32 channels = 0;

    Texture(const std::string& path);
    ~Texture();

    void bind() const;
};

