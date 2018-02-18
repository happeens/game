#include <render_group.hpp>

#include <stdio.h>

#include <resource_manager.hpp>

std::shared_ptr<RenderGroup> RenderGroup::primitive(glm::mat4 projection) {
    auto result = std::make_shared<RenderGroup>();
    result->type = RenderGroupType::Primitive;

    auto shader = ResourceManager::get_instance().get_shader("primitive");
    shader->use();
    shader->set_uniform("projection", projection);

    result->shader = shader;
    result->projection = projection;

    glGenVertexArrays(1, &result->vao);
    glBindVertexArray(result->vao);

    glGenBuffers(1, &result->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, result->vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        MAX_RECT_COUNT * VBO_RECT_ELEMS * sizeof(GLfloat),
        nullptr,
        GL_DYNAMIC_DRAW
    );

    glGenBuffers(1, &result->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result->ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        MAX_RECT_COUNT * EBO_RECT_ELEMS * sizeof(GLuint),
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
    glm::mat4 projection,
    std::shared_ptr<Texture> texture
) {
    auto result = std::make_shared<RenderGroup>();
    result->type = RenderGroupType::Sprite;

    auto shader = ResourceManager::get_instance().get_shader("sprite");
    shader->use();
    shader->set_uniform("projection", projection);

    result->shader = shader;
    result->projection = projection;
    result->texture = texture;

    glGenVertexArrays(1, &result->vao);
    glBindVertexArray(result->vao);

    glGenBuffers(1, &result->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, result->vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        MAX_RECT_COUNT * VBO_RECT_ELEMS * sizeof(GLfloat),
        nullptr,
        GL_DYNAMIC_DRAW
    );

    glGenBuffers(1, &result->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result->ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        MAX_RECT_COUNT * EBO_RECT_ELEMS * sizeof(GLuint),
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
RenderGroup::~RenderGroup() {}

static void draw_primitive_group(const RenderGroup* group) {
    // TODO: keep this in a buffer to avoid reallocation every frame?
    // NOTE: (2 * 4) verts per rect, (3 * 4) color points per rect
    f32 vertices[MAX_RECT_COUNT * (8 + 12)] = {};
    GLuint elements[MAX_RECT_COUNT * 6] = {};

    for (u32 i = 0; i < group->rect_count; i++) {
        auto rect = std::get<ColoredRect>(group->rects[i]);
        auto vert_index = i * 8;

        // top left
        vertices[vert_index] = (f32) rect.x_min;
        vertices[vert_index + 1] = (f32) rect.y_min;

        vertices[vert_index + 2] = 1.0;
        vertices[vert_index + 3] = 1.0;
        vertices[vert_index + 4] = 0.0;

        // top right
        vertices[vert_index + 5] = (f32) rect.x_max;
        vertices[vert_index + 6] = (f32) rect.y_min;

        vertices[vert_index + 7] = 0.0;
        vertices[vert_index + 8] = 1.0;
        vertices[vert_index + 9] = 1.0;

        // bottom left
        vertices[vert_index + 10] = (f32) rect.x_min;
        vertices[vert_index + 11] = (f32) rect.y_max;

        vertices[vert_index + 12] = 1.0;
        vertices[vert_index + 13] = 0.0;
        vertices[vert_index + 14] = 1.0;

        // bottom right
        vertices[vert_index + 15] = (f32) rect.x_max;
        vertices[vert_index + 16] = (f32) rect.y_max;

        vertices[vert_index + 17] = 0.0;
        vertices[vert_index + 18] = 0.0;
        vertices[vert_index + 19] = 1.0;

        auto elem_index = i * 6;
        auto elem = i * 4;
        elements[elem_index] = elem;
        elements[elem_index + 1] = elem + 1;
        elements[elem_index + 2] = elem + 2;
        elements[elem_index + 3] = elem + 1;
        elements[elem_index + 4] = elem + 2;
        elements[elem_index + 5] = elem + 3;
    }

    glBufferSubData(
        GL_ARRAY_BUFFER,
        0, group->rect_count * (8 + 12) * sizeof(GLfloat),
        vertices
    );

    glBufferSubData(
        GL_ELEMENT_ARRAY_BUFFER,
        0, group->rect_count * 6 * sizeof(GLuint),
        elements
    );

    glDrawElements(
        GL_TRIANGLES,
        group->rect_count * 6,
        GL_UNSIGNED_INT,
        0
    );
}

static void draw_sprite_group(const RenderGroup* group) {
    // TODO: keep this in a buffer to avoid reallocation every frame?
    f32 vertices[MAX_RECT_COUNT * 8] = {};
    GLuint elements[MAX_RECT_COUNT * 6] = {};

    group->texture->bind();

    for (u32 i = 0; i < group->rect_count; i++) {
        auto rect = std::get<TexturedRect>(group->rects[i]);
        auto vert_index = i * 8;

        // top left
        vertices[vert_index] = (f32) rect.x_min;
        vertices[vert_index + 1] = (f32) rect.y_min;

        // top right
        vertices[vert_index + 2] = (f32) rect.x_max;
        vertices[vert_index + 3] = (f32) rect.y_min;

        // bottom left
        vertices[vert_index + 4] = (f32) rect.x_min;
        vertices[vert_index + 5] = (f32) rect.y_max;

        // bottom right
        vertices[vert_index + 6] = (f32) rect.x_max;
        vertices[vert_index + 7] = (f32) rect.y_max;

        auto elem_index = i * 6;
        auto elem = i * 4;
        elements[elem_index] = elem;
        elements[elem_index + 1] = elem + 1;
        elements[elem_index + 2] = elem + 2;
        elements[elem_index + 3] = elem + 1;
        elements[elem_index + 4] = elem + 2;
        elements[elem_index + 5] = elem + 3;
    }

    glBufferSubData(
        GL_ARRAY_BUFFER,
        0, group->rect_count * 8 * sizeof(GLfloat),
        vertices
    );

    glBufferSubData(
        GL_ELEMENT_ARRAY_BUFFER,
        0, group->rect_count * 6 * sizeof(GLuint),
        elements
    );

    glDrawElements(
        GL_TRIANGLES,
        group->rect_count * 6,
        GL_UNSIGNED_INT,
        0
    );
}

void RenderGroup::draw() const {
    this->shader->use();
    this->shader->set_uniform("projection", this->projection);

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

void RenderGroup::push_rect(Rect rect) {
    this->rects[this->rect_count] = rect;
    this->rect_count++;
}

