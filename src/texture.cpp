#include <texture.hpp>

#include <stdio.h>
#include <cmath>
#include <algorithm>

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

struct Anchor {
    u32 x;
    u32 y;

    Anchor(u32 x = 0, u32 y = 0) : x(x), y(y) {};
};

static inline bool compare_pow(
    const Anchor& lhs, const Anchor& rhs
) {
    return (
        (std::pow((double) lhs.x, 4) + std::pow((double) lhs.y, 4)) <
        (std::pow((double) rhs.x, 4) + std::pow((double) rhs.y, 4))
    );
}

struct BatchSprite {
    u32 width;
    u32 height;
    u32 channels;
    unsigned char* data;
};

static inline bool compare_area(
    const BatchSprite& lhs, const BatchSprite& rhs
) {
    return (
        (lhs.width * lhs.height) > (rhs.width * rhs.height)
    );
}

Texture::Texture(const std::vector<std::string>& batch) {
    ASSERT(batch.size() > 1);

    std::vector<BatchSprite> sprites = {};
    for (auto it : batch) {
        BatchSprite sprite = {};
        i32 width, height, channels;

        sprite.data = stbi_load(
            it.c_str(),
            &width, &height,
            &channels, STBI_rgb_alpha
        );

        if (!sprite.data) {
            printf("failed to load image %s\n", it.c_str());
            return;
        }

        sprite.width = (u32) width;
        sprite.height = (u32) height;
        sprite.channels = (u32) channels;

        sprites.push_back(sprite);
        printf("found sprite %d/%d\n", width, height);
    }

    std::sort(sprites.begin(), sprites.end(), compare_area);

    std::vector<Anchor> free_anchors = {};
    std::vector<Anchor> abs_anchors = {};

    free_anchors.push_back(Anchor());

    for (u32 i = 0; i < sprites.size(); ++i) {
        abs_anchors.push_back(free_anchors.front());

        auto new_anchor_right = Anchor(
            free_anchors.front().x + sprites[i].width,
            free_anchors.front().y
        );
        auto new_anchor_bottom = Anchor(
            free_anchors.front().x,
            free_anchors.front().y + sprites[i].height
        );

        for (u32 j = 1; j < free_anchors.size(); ++j) {
            if (
                (free_anchors[j].x >= free_anchors.front().x) &&
                (free_anchors[j].x <= new_anchor_right.x)
            ) {
                new_anchor_right.y = std::min(
                    new_anchor_right.y,
                    free_anchors[j].y
                );
                free_anchors.erase(free_anchors.begin() + j);
                continue;
            }

            if (
                (free_anchors[j].y >= free_anchors.front().y) &&
                (free_anchors[j].y <= new_anchor_bottom.y)
            ) {
                new_anchor_bottom.x = std::min(
                    new_anchor_bottom.x,
                    free_anchors[j].x
                );
                free_anchors.erase(free_anchors.begin() + j);
                continue;
            }
        }

        free_anchors.erase(free_anchors.begin());
        free_anchors.push_back(new_anchor_right);
        free_anchors.push_back(new_anchor_bottom);

        std::sort(free_anchors.begin(), free_anchors.end(), compare_pow);
    }

    this->width = 0;
    this->height = 0;
    for (auto it : free_anchors) {
        this->width = std::max(width, it.x);
        this->height = std::max(height, it.y);
    }

    std::vector<unsigned char> result(width * height * 4, 0);

    for (u32 i = 0; i < abs_anchors.size(); ++i) {
        auto sprite = sprites[i];
        auto anchor = abs_anchors[i];
        auto anchor_offset = (anchor.y * this->width * 4) + anchor.x * 4;

        for (u32 y = 0; y < sprite.height; ++y) {
            auto sprite_height_offset = y * sprite.width * 4;
            auto batch_height_offset = y * this->width * 4;

            for (u32 x = 0; x < sprite.width * 4; ++x) {
                result[anchor_offset + batch_height_offset + x] =
                    sprite.data[sprite_height_offset + x];
            }
        }
    }

    glGenTextures(1, &this->id);
    glBindTexture(GL_TEXTURE_2D, this->id);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA,
        this->width, this->height, 0, GL_RGBA,
        GL_UNSIGNED_BYTE, result.data()
    );

    for (auto sprite : sprites)
        stbi_image_free(sprite.data);

    auto sprite = SpriteData(
        Position(0.0f, 0.0f),
        Size((f32) this->width, (f32) this->height)
    );

    this->sprites.push_back(sprite);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    glDeleteTextures(1, &this->id);
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, this->id);
}

