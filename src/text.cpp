#include <text.hpp>

#include <stdio.h>
#include <fstream>
#include <iterator>
#include <algorithm>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>

FontTexture::FontTexture() {
    std::vector<char> font_data;
    std::ifstream font_file;

    font_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        font_file.open("../assets/visitor.ttf");

        font_data.insert(
            font_data.begin(),
            std::istreambuf_iterator<char>(font_file),
            std::istreambuf_iterator<char>()
        );

        font_file.close();
    } catch (std::ifstream::failure e) {
        printf("error: font file could not be read\n");
        return;
    }

    stbtt_fontinfo font_info;
    auto init_result = stbtt_InitFont(
        &font_info,
        reinterpret_cast<unsigned char*>(font_data.data()),
        0
    );

    if (!init_result) {
        printf("failed to init font\n");
        return;
    }

    auto scale = stbtt_ScaleForPixelHeight(&font_info, 50);

    // get metrics
    i32 ascent, descent, line_gap;
    stbtt_GetFontVMetrics(&font_info, &ascent, &descent, &line_gap);

    ascent *= scale;
    descent *= scale;

    u32 x_offset = 0;
    i32 max_y = 0;

    for (u32 c = 'a'; c <= 'z'; c++) {
        CharacterData character = {};
        character.pos_x = x_offset;
        // TODO: render character at correct height
        character.pos_y = 0;

        // get bounding box
        i32 x1, y1, x2, y2;
        stbtt_GetCodepointBitmapBox(
            &font_info, c, scale, scale,
            &x1, &y1, &x2, &y2
        );

        i32 width = x2 - x1;
        i32 height = y2 - y1;

        ASSERT(width > 0);
        ASSERT(height > 0);

        character.width = (u32) width;
        character.height = (u32) height;

        max_y = std::max(max_y, height);
        x_offset += width;

        this->characters[c] = character;
    }

    auto result_width = x_offset;
    auto result_height = max_y;
    auto bitmap = new unsigned char[result_width * result_height] {0};

    for (auto& it : this->characters) {
        printf("printing %c\n", it.first);
        auto offset = it.second.pos_x + (result_height * it.second.pos_y);

        stbtt_MakeCodepointBitmap(
            &font_info, bitmap + offset, it.second.width, it.second.height,
            result_width, scale, scale, it.first
        );
    }

    ASSERT(result_width > 0);
    ASSERT(result_height > 0);

    this->width = (u32) result_width;
    this->height = (u32) result_height;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &this->id);
    glBindTexture(GL_TEXTURE_2D, this->id);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA,
        this->width, this->height, 0, GL_LUMINANCE,
        GL_UNSIGNED_BYTE, bitmap
    );

    delete bitmap;
    glBindTexture(GL_TEXTURE_2D, 0);
}

FontTexture::~FontTexture() {}

void FontTexture::bind() const {
    glBindTexture(GL_TEXTURE_2D, this->id);
}

