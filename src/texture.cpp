#include <texture.hpp>

#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Texture::Texture(const std::string& path) {
    i32 width, height, channels;

    unsigned char* data = stbi_load(
        path.c_str(),
        &width, &height,
        &channels, STBI_rgb_alpha
    );

    if (!data) {
        printf("failed to load image %s\n", path.c_str());
        return;
    }

    this->width = (u32) width;
    this->height = (u32) height;
    this->channels = (u32) channels;

    for (u32 i = 0; i < 3; i++) {
        auto sprite = SpriteData(
            Position(i * 30.0f, 0.0f),
            Size(30.0f, 30.0f)
        );
        this->sprites.push_back(sprite);
    }

    glGenTextures(1, &this->id);
    glBindTexture(GL_TEXTURE_2D, this->id);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA,
        this->width, this->height, 0, GL_RGBA,
        GL_UNSIGNED_BYTE, data
    );

    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    glDeleteTextures(1, &this->id);
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, this->id);
}

