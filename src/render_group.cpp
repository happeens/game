#include <render_group.hpp>

RenderGroup::RenderGroup() {
    this->rect_count = 0;

    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        MAX_RECT_COUNT * VBO_RECT_ELEMS * sizeof(GLfloat),
        nullptr,
        GL_DYNAMIC_DRAW
    );

    glGenBuffers(1, &this->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        MAX_RECT_COUNT * EBO_RECT_ELEMS * sizeof(GLuint),
        nullptr,
        GL_DYNAMIC_DRAW
    );

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

    glBindVertexArray(0);
    return;
}

RenderGroup::~RenderGroup() {}

void RenderGroup::draw() const {
    glBindVertexArray(this->vao);

    auto rect_count = this->rect_count;
    auto rects = &this->rects;

    f32 vertices[MAX_RECT_COUNT * 8] = {};
    GLuint elements[MAX_RECT_COUNT * 6] = {};

    for (u32 i = 0; i < rect_count; i++) {
        auto vert_index = i * 8;
        vertices[vert_index] = (f32) rects[i]->x_min;
        vertices[vert_index + 1] = (f32) rects[i]->y_min;

        vertices[vert_index + 2] = (f32) rects[i]->x_max;
        vertices[vert_index + 3] = (f32) rects[i]->y_min;

        vertices[vert_index + 4] = (f32) rects[i]->x_min;
        vertices[vert_index + 5] = (f32) rects[i]->y_max;

        vertices[vert_index + 6] = (f32) rects[i]->x_max;
        vertices[vert_index + 7] = (f32) rects[i]->y_max;

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
        0, rect_count * 8 * sizeof(GLfloat),
        vertices
    );

    glBufferSubData(
        GL_ELEMENT_ARRAY_BUFFER,
        0, rect_count * 6 * sizeof(GLuint),
        elements
    );

    glDrawElements(
        GL_TRIANGLES,
        rect_count * 6,
        GL_UNSIGNED_INT,
        0
    );

    glBindVertexArray(0);
}

