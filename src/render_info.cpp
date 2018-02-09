#include <render_info.hpp>

RenderInfo::RenderInfo(GLFWwindow* window) {
    this->renderer_string = glGetString(GL_RENDERER);
    this->version_string = glGetString(GL_VERSION);

    // printf("renderer: %s, opengl version: %s\n", renderer_string, version_string);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glfwGetFramebufferSize(
        window,
        &this->viewport_width,
        &this->viewport_height
    );

    glViewport(0, 0, this->viewport_width, this->viewport_height);
}

RenderInfo::~RenderInfo() {
    for (u32 i = 0; i < this->render_group_count; i++) {
        delete this->render_groups[i];
    }
}

void RenderInfo::draw() const {
    for (u32 i = 0; i < this->render_group_count; i++) {
        this->render_groups[i]->draw();
    }
}

void RenderInfo::push_render_group(RenderGroup* render_group) {
    this->render_groups[this->render_group_count] = render_group;
    this->render_group_count++;
}
