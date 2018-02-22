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
        MAX_RECT_COUNT * 8 * sizeof(GLfloat),
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
        2 * sizeof(GLfloat), (void*) 0
    );

    glBindVertexArray(0);
    return result;
}

RenderGroup::RenderGroup() {}

RenderGroup::~RenderGroup() {
    glDeleteVertexArrays(1, &this->vao);
}

static void draw_primitive_group(const RenderGroup* group) {
    // TODO: keep this in a buffer to avoid reallocation every frame?
    // NOTE: (2 * 4) verts per rect, (3 * 4) color points per rect
    std::vector<f32> vertices = {};
    vertices.reserve(group->rects.size() * 20);

    std::vector<GLuint> elements = {};
    elements.reserve(group->rects.size() * 6);

    for (u32 i = 0; i < group->rects.size(); i++) {
        auto rect = std::get<ColoredRect>(group->rects[i]);
        auto vert_index = i * 20;

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
        0, group->rects.size() * 20 * sizeof(GLfloat),
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
    // TODO: keep this in a buffer to avoid reallocation every frame?
    std::vector<f32> vertices = {};
    vertices.reserve(group->rects.size() * 8);

    std::vector<GLuint> elements = {};
    elements.reserve(group->rects.size() * 6);

    (*group->texture)->bind();

    for (u32 i = 0; i < group->rects.size(); i++) {
        auto rect = std::get<TexturedRect>(group->rects[i]);
        auto vert_index = i * 8;

        // top left
        vertices[vert_index] = (f32) rect.pos.x;
        vertices[vert_index + 1] = (f32) rect.pos.y;

        // top right
        vertices[vert_index + 2] = (f32) rect.pos.x + rect.size.width;
        vertices[vert_index + 3] = (f32) rect.pos.y;

        // bottom left
        vertices[vert_index + 4] = (f32) rect.pos.x;
        vertices[vert_index + 5] = (f32) rect.pos.y + rect.size.height;

        // bottom right
        vertices[vert_index + 6] = (f32) rect.pos.x + rect.size.width;
        vertices[vert_index + 7] = (f32) rect.pos.y + rect.size.height;

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
        0, group->rects.size() * 8 * sizeof(GLfloat),
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

