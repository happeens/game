#include <render_context.hpp>

#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <resource_manager.hpp>

RenderContext::RenderContext(GLFWwindow* window) {
    this->renderer_string = glGetString(GL_RENDERER);
    this->version_string = glGetString(GL_VERSION);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    Viewport viewport = {};

    i32 viewport_width = 0;
    i32 viewport_height = 0;

    glfwGetFramebufferSize(
        window,
        &viewport_width,
        &viewport_height
    );

    viewport.width = (u32) viewport_width;
    viewport.height = (u32) viewport_height;

    viewport.projection = glm::ortho(
        0.0f,
        (f32) viewport_width,
        (f32) viewport_height,
        0.0f
    );

    glGenBuffers(1, &viewport.ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, viewport.ubo);
    glBufferData(
        GL_UNIFORM_BUFFER,
        sizeof(glm::mat4) + (2 * sizeof(u32)),
        nullptr,
        GL_STATIC_DRAW
    );

    glBufferSubData(
        GL_UNIFORM_BUFFER,
        0, sizeof(glm::mat4),
        glm::value_ptr(viewport.projection)
    );

    glBufferSubData(
        GL_UNIFORM_BUFFER,
        sizeof(glm::mat4), sizeof(u32),
        (void*) &viewport.width
    );

    glBufferSubData(
        GL_UNIFORM_BUFFER,
        sizeof(glm::mat4) + sizeof(u32), sizeof(u32),
        (void*) &viewport.height
    );
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // NOTE: right now, we only use the buffer index for
    // a single object, so its always bound to 0. There should
    // be global indices if more are being used.
    glBindBufferRange(
        GL_UNIFORM_BUFFER, 0, 
        viewport.ubo,
        0, sizeof(glm::mat4)
    );

    this->viewport = viewport;

    glViewport(0, 0, viewport_width, viewport_height);
}

RenderContext::~RenderContext() {}

void RenderContext::draw() const {
    for (auto it : this->render_groups)
        it->draw();
}

std::shared_ptr<RenderGroup> RenderContext::create_primitive_render_group() {
    auto render_group = RenderGroup::primitive(this->viewport);
    this->render_groups.push_back(render_group);
    return render_group;
}

std::shared_ptr<RenderGroup> RenderContext::create_sprite_render_group(
    const std::string& texture_name
) {
    auto texture = ResourceManager::get_instance().get_texture(texture_name);
    texture->bind();

    // TODO: make this configurable
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    auto render_group = RenderGroup::sprite(this->viewport, texture);
    this->render_groups.push_back(render_group);
    return render_group;
}
