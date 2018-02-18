#include <render_info.hpp>

#include <stdio.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <resource_manager.hpp>

RenderInfo::RenderInfo(GLFWwindow* window) {
    this->renderer_string = glGetString(GL_RENDERER);
    this->version_string = glGetString(GL_VERSION);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glfwGetFramebufferSize(
        window,
        &this->viewport_width,
        &this->viewport_height
    );

    glViewport(0, 0, this->viewport_width, this->viewport_height);
}

RenderInfo::~RenderInfo() {}

void RenderInfo::draw() const {
    for (u32 i = 0; i < this->render_group_count; i++) {
        this->render_groups[i]->draw();
    }
}

std::shared_ptr<RenderGroup> RenderInfo::create_primitive_render_group() {
    glm::mat4 proj = glm::ortho(
        0.0f,
        (f32) this->viewport_width,
        (f32) this->viewport_height,
        0.0f
    );

    auto render_group = RenderGroup::primitive(proj);
    this->render_groups[this->render_group_count] = render_group;
    this->render_group_count++;
    return render_group;
}

std::shared_ptr<RenderGroup> RenderInfo::create_sprite_render_group() {
    glm::mat4 proj = glm::ortho(
        0.0f,
        (f32) this->viewport_width,
        (f32) this->viewport_height,
        0.0f
    );

    auto texture = ResourceManager::get_instance().get_texture("wall.jpg");
    texture->bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    auto render_group = RenderGroup::sprite(proj, texture);
    this->render_groups[this->render_group_count] = render_group;
    this->render_group_count++;
    return render_group;
}
