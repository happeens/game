#include <render_group.hpp>

#include <stdio.h>

#include <resource_manager.hpp>

std::shared_ptr<RenderGroup> RenderGroup::primitive(Viewport viewport) {
    auto result = std::make_shared<RenderGroup>();
    result->type = RenderGroupType::Primitive;

    auto shader = ResourceManager::get_instance().get_shader("primitive");
    shader->use();
    shader->bind_uniform_block("Viewport", viewport.ubo);

    result->texture = std::nullopt;
    result->shader = shader;

    glGenVertexArrays(1, &result->vao);
    glBindVertexArray(result->vao);

    glGenBuffers(1, &result->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, result->vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        MAX_RECT_COUNT * 20 * sizeof(GLfloat),
        nullptr,
        GL_DYNAMIC_DRAW
    );

    glGenBuffers(1, &result->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result->ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        MAX_RECT_COUNT * 6 * sizeof(GLuint),
        nullptr,
        GL_DYNAMIC_DRAW
    );

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE,
        5 * sizeof(GLfloat), (void*) 0
    );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE,
        5 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat))
    );

    glBindVertexArray(0);
    return result;
}

std::shared_ptr<RenderGroup> RenderGroup::sprite(
    Viewport viewport,
    std::shared_ptr<Texture> texture
) {
    auto result = std::make_shared<RenderGroup>();
    result->type = RenderGroupType::Sprite;

    auto shader = ResourceManager::get_instance().get_shader("sprite");
    shader->use();
    shader->bind_uniform_block("Viewport", viewport.ubo);

    result->texture = texture;
    result->shader = shader;

    glGenVertexArrays(1, &result->vao);
    glBindVertexArray(result->vao);

    glGenBuffers(1, &result->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, result->vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        MAX_RECT_COUNT * 24 * sizeof(GLfloat),
        nullptr,
        GL_DYNAMIC_DRAW
    );

    glGenBuffers(1, &result->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result->ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        MAX_RECT_COUNT * 6 * sizeof(GLuint),
        nullptr,
        GL_DYNAMIC_DRAW
    );

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE,
        6 * sizeof(GLfloat), (void*) 0
    );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE,
        6 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat))
    );

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE,
        6 * sizeof(GLfloat), (void*) (4 * sizeof(GLfloat))
    );

    glBindVertexArray(0);
    return result;
}

std::shared_ptr<RenderGroup> RenderGroup::text(
    Viewport viewport,
    std::shared_ptr<FontTexture> font_texture
) {
    auto result = std::make_shared<RenderGroup>();
    result->type = RenderGroupType::Text;

    auto shader = ResourceManager::get_instance().get_shader("text");
    shader->use();
    shader->bind_uniform_block("Viewport", viewport.ubo);

    result->font_texture = font_texture;
    result->shader = shader;

    glGenVertexArrays(1, &result->vao);
    glBindVertexArray(result->vao);

    glGenBuffers(1, &result->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, result->vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        MAX_RECT_COUNT * 4 * 9 * sizeof(GLfloat),
        nullptr,
        GL_DYNAMIC_DRAW
    );

    glGenBuffers(1, &result->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result->ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        MAX_RECT_COUNT * 6 * sizeof(GLuint),
        nullptr,
        GL_DYNAMIC_DRAW
    );

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE,
        9 * sizeof(GLfloat), (void*) 0
    );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE,
        9 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat))
    );

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE,
        9 * sizeof(GLfloat), (void*) (5 * sizeof(GLfloat))
    );

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(
        3, 2, GL_FLOAT, GL_FALSE,
        9 * sizeof(GLfloat), (void*) (7 * sizeof(GLfloat))
    );

    glBindVertexArray(0);
    return result;
}

RenderGroup::RenderGroup() {}

RenderGroup::~RenderGroup() {
    // TODO: more thorough cleanup
    glDeleteVertexArrays(1, &this->vao);
}

static void draw_primitive_group(const RenderGroup* group) {
    // TODO: keep this in a buffer to avoid reallocation every frame?
    // NOTE: (2 * 4) verts per rect, (3 * 4) color points per rect
    std::vector<f32> vertices = {};
    vertices.reserve(group->rects.size() * 4 * 5);

    std::vector<GLuint> elements = {};
    elements.reserve(group->rects.size() * 6);

    for (u32 i = 0; i < group->rects.size(); i++) {
        auto rect = std::get<ColoredRect>(group->rects[i]);
        auto vert_index = i * 4 * 5;

        // top left
        vertices[vert_index] = (f32) rect.pos.x;
        vertices[vert_index + 1] = (f32) rect.pos.y;

        vertices[vert_index + 2] = rect.color.r;
        vertices[vert_index + 3] = rect.color.g;
        vertices[vert_index + 4] = rect.color.b;

        // top right
        vertices[vert_index + 5] = (f32) rect.pos.x + rect.size.width;
        vertices[vert_index + 6] = (f32) rect.pos.y;

        vertices[vert_index + 7] = rect.color.r;
        vertices[vert_index + 8] = rect.color.g;
        vertices[vert_index + 9] = rect.color.b;

        // bottom left
        vertices[vert_index + 10] = (f32) rect.pos.x;
        vertices[vert_index + 11] = (f32) rect.pos.y + rect.size.height;

        vertices[vert_index + 12] = rect.color.r;
        vertices[vert_index + 13] = rect.color.g;
        vertices[vert_index + 14] = rect.color.b;

        // bottom right
        vertices[vert_index + 15] = (f32) rect.pos.x + rect.size.width;
        vertices[vert_index + 16] = (f32) rect.pos.y + rect.size.height;

        vertices[vert_index + 17] = rect.color.r;
        vertices[vert_index + 18] = rect.color.g;
        vertices[vert_index + 19] = rect.color.b;

        auto elem_index = i * 6;
        auto elem = i * 4;
        elements[elem_index] = elem;
        elements[elem_index + 1] = elem + 1;
        elements[elem_index + 2] = elem + 2;
        elements[elem_index + 3] = elem + 1;
        elements[elem_index + 4] = elem + 2;
        elements[elem_index + 5] = elem + 3;
    }

    glBindBuffer(GL_ARRAY_BUFFER, group->vbo);
    glBufferSubData(
        GL_ARRAY_BUFFER,
        0, group->rects.size() * 4 * 5 * sizeof(GLfloat),
        vertices.data()
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, group->ebo);
    glBufferSubData(
        GL_ELEMENT_ARRAY_BUFFER,
        0, group->rects.size() * 6 * sizeof(GLuint),
        elements.data()
    );

    glDrawElements(
        GL_TRIANGLES,
        group->rects.size() * 6,
        GL_UNSIGNED_INT,
        0
    );
}

static void draw_sprite_group(const RenderGroup* group) {
    ASSERT((bool) group->texture);

    // TODO: keep this in a buffer to avoid reallocation every frame?
    std::vector<f32> vertices = {};
    vertices.reserve(group->rects.size() * 4 * 6);

    std::vector<GLuint> elements = {};
    elements.reserve(group->rects.size() * 6);

    auto texture = *group->texture;
    texture->bind();

    for (u32 i = 0; i < group->rects.size(); i++) {
        auto rect = std::get<TexturedRect>(group->rects[i]);
        auto vert_index = i * 4 * 6;
        auto sprite_data = texture->sprites[rect.active_sprite];

        auto tex_size_x = sprite_data.tex_size.width / (f32) texture->width;
        auto tex_size_y = sprite_data.tex_size.height / (f32) texture->height;

        auto tex_offset_x = sprite_data.tex_pos.x / (f32) texture->width;
        auto tex_offset_y = sprite_data.tex_pos.y / (f32) texture->height;

        // top left
        vertices[vert_index] = rect.pos.x;
        vertices[vert_index + 1] = rect.pos.y;

        vertices[vert_index + 2] = tex_size_x;
        vertices[vert_index + 3] = tex_size_y;

        vertices[vert_index + 4] = tex_offset_x;
        vertices[vert_index + 5] = tex_offset_y;

        // top right
        vertices[vert_index + 6] = rect.pos.x + rect.size.width;
        vertices[vert_index + 7] = rect.pos.y;

        vertices[vert_index + 8] = tex_size_x;
        vertices[vert_index + 9] = tex_size_y;

        vertices[vert_index + 10] = tex_offset_x;
        vertices[vert_index + 11] = tex_offset_y;

        // bottom left
        vertices[vert_index + 12] = rect.pos.x;
        vertices[vert_index + 13] = rect.pos.y + rect.size.height;

        vertices[vert_index + 14] = tex_size_x;
        vertices[vert_index + 15] = tex_size_y;

        vertices[vert_index + 16] = tex_offset_x;
        vertices[vert_index + 17] = tex_offset_y;

        // bottom right
        vertices[vert_index + 18] = rect.pos.x + rect.size.width;
        vertices[vert_index + 19] = rect.pos.y + rect.size.height;

        vertices[vert_index + 20] = tex_size_x;
        vertices[vert_index + 21] = tex_size_y;

        vertices[vert_index + 22] = tex_offset_x;
        vertices[vert_index + 23] = tex_offset_y;

        auto elem_index = i * 6;
        auto elem = i * 4;
        elements[elem_index + 0] = elem + 0;
        elements[elem_index + 1] = elem + 1;
        elements[elem_index + 2] = elem + 2;
        elements[elem_index + 3] = elem + 1;
        elements[elem_index + 4] = elem + 2;
        elements[elem_index + 5] = elem + 3;
    }

    glBindBuffer(GL_ARRAY_BUFFER, group->vbo);
    glBufferSubData(
        GL_ARRAY_BUFFER,
        0, group->rects.size() * 4 * 6 * sizeof(GLfloat),
        vertices.data()
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, group->ebo);
    glBufferSubData(
        GL_ELEMENT_ARRAY_BUFFER,
        0, group->rects.size() * 6 * sizeof(GLuint),
        elements.data()
    );

    glDrawElements(
        GL_TRIANGLES,
        group->rects.size() * 6,
        GL_UNSIGNED_INT,
        0
    );
}

static void draw_text_group(const RenderGroup* group) {
    ASSERT((bool) group->font_texture);
    auto font_texture = *group->font_texture;
    font_texture->bind();

    std::vector<f32> vertices = {};
    vertices.reserve(group->rects.size() * 4 * 9);

    std::vector<GLuint> elements = {};
    elements.reserve(group->rects.size() * 6);

    for (u32 i = 0; i < group->rects.size(); i++) {
        auto rect = std::get<CharacterRect>(group->rects[i]);
        auto& char_data = font_texture->characters[rect.character];
        auto vert_index = i * 4 * 9;

        auto tex_size_x = (f32) char_data.width / (f32) font_texture->width;
        auto tex_size_y = (f32) char_data.height / (f32) font_texture->height;

        auto tex_offset_x = (f32) char_data.pos_x / (f32) font_texture->width;
        auto tex_offset_y = (f32) char_data.pos_y / (f32) font_texture->height;

        // top left
        vertices[vert_index] = rect.pos.x;
        vertices[vert_index + 1] = rect.pos.y;

        vertices[vert_index + 2] = rect.color.r;
        vertices[vert_index + 3] = rect.color.g;
        vertices[vert_index + 4] = rect.color.b;

        vertices[vert_index + 5] = tex_size_x;
        vertices[vert_index + 6] = tex_size_y;

        vertices[vert_index + 7] = tex_offset_x;
        vertices[vert_index + 8] = tex_offset_y;

        // top right
        vertices[vert_index + 9] =
            rect.pos.x + (f32) char_data.width * rect.scale;
        vertices[vert_index + 10] = rect.pos.y;

        vertices[vert_index + 11] = rect.color.r;
        vertices[vert_index + 12] = rect.color.g;
        vertices[vert_index + 13] = rect.color.b;

        vertices[vert_index + 14] = tex_size_x;
        vertices[vert_index + 15] = tex_size_y;

        vertices[vert_index + 16] = tex_offset_x;
        vertices[vert_index + 17] = tex_offset_y;

        // bottom left
        vertices[vert_index + 18] = rect.pos.x;
        vertices[vert_index + 19] =
            rect.pos.y + (f32) char_data.height * rect.scale;

        vertices[vert_index + 20] = rect.color.r;
        vertices[vert_index + 21] = rect.color.g;
        vertices[vert_index + 22] = rect.color.b;

        vertices[vert_index + 23] = tex_size_x;
        vertices[vert_index + 24] = tex_size_y;

        vertices[vert_index + 25] = tex_offset_x;
        vertices[vert_index + 26] = tex_offset_y;

        // bottom right
        vertices[vert_index + 27] =
            rect.pos.x + (f32) char_data.width * rect.scale;
        vertices[vert_index + 28] =
            rect.pos.y + (f32) char_data.height * rect.scale;

        vertices[vert_index + 29] = rect.color.r;
        vertices[vert_index + 30] = rect.color.g;
        vertices[vert_index + 31] = rect.color.b;

        vertices[vert_index + 32] = tex_size_x;
        vertices[vert_index + 33] = tex_size_y;

        vertices[vert_index + 34] = tex_offset_x;
        vertices[vert_index + 35] = tex_offset_y;

        auto elem_index = i * 6;
        auto elem = i * 4;
        elements[elem_index + 0] = elem + 0;
        elements[elem_index + 1] = elem + 1;
        elements[elem_index + 2] = elem + 2;
        elements[elem_index + 3] = elem + 1;
        elements[elem_index + 4] = elem + 2;
        elements[elem_index + 5] = elem + 3;
    }

    glBindBuffer(GL_ARRAY_BUFFER, group->vbo);
    glBufferSubData(
        GL_ARRAY_BUFFER,
        0, group->rects.size() * 4 * 9 * sizeof(GLfloat),
        vertices.data()
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, group->ebo);
    glBufferSubData(
        GL_ELEMENT_ARRAY_BUFFER,
        0, group->rects.size() * 6 * sizeof(GLuint),
        elements.data()
    );

    glDrawElements(
        GL_TRIANGLES,
        group->rects.size() * 6,
        GL_UNSIGNED_INT,
        0
    );
}

void RenderGroup::draw() const {
    this->shader->use();

    glBindVertexArray(this->vao);

    switch (this->type) {
    case RenderGroupType::Primitive:
        draw_primitive_group(this);
        break;
    case RenderGroupType::Sprite:
        draw_sprite_group(this);
        break;
    case RenderGroupType::Text:
        draw_text_group(this);
        break;
    }

    glBindVertexArray(0);
}

void RenderGroup::push_rect(ColoredRect rect) {
    ASSERT(this->type == RenderGroupType::Primitive);

    this->rects.push_back(rect);
}

void RenderGroup::push_rect(TexturedRect rect) {
    ASSERT(this->type == RenderGroupType::Sprite);
    ASSERT((bool) this->texture);

    this->rects.push_back(rect);
}

void RenderGroup::push_rect(CharacterRect rect) {
    ASSERT(this->type == RenderGroupType::Text);
    ASSERT((bool) this->font_texture);

    this->rects.push_back(rect);
}

