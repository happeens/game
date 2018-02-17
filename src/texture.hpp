#ifndef __TEXTURE__
#define __TEXTURE__

#include <stdio.h>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

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

#endif

