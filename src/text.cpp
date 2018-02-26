#include <text.hpp>

#include <stdio.h>
#include <fstream>
#include <iterator>

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

    // get bounding box
    i32 x1, y1, x2, y2;
    stbtt_GetCodepointBitmapBox(
        &font_info, 'A', scale, scale,
        &x1, &y1, &x2, &y2
    );

    i32 width = x2 - x1;
    i32 height = y2 - y1;

    auto y = ascent + y1;
    auto offset = y * 50;

    ASSERT(width < 50);
    ASSERT(height < 50);

    /* auto bitmap = new unsigned char[50 * 50] {0}; */
    unsigned char bitmap[50 * 50] = {0};

    stbtt_MakeCodepointBitmap(
        &font_info, bitmap, 50, 50,
        50, scale, scale, 'A'
    );

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &this->id);
    glBindTexture(GL_TEXTURE_2D, this->id);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA,
        50, 50, 0, GL_LUMINANCE,
        GL_UNSIGNED_BYTE, &bitmap
    );

    glBindTexture(GL_TEXTURE_2D, 0);
    /* delete bitmap; */
}

FontTexture::~FontTexture() {}

void FontTexture::bind() const {
    glBindTexture(GL_TEXTURE_2D, this->id);
}

