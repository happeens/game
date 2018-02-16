#include <render_info.hpp>

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
    auto shader = ResourceManager::get_instance().get_shader("primitive");
    shader->use();

    glm::mat4 proj = glm::ortho(
        0.0f,
        (f32) this->viewport_width,
        (f32) this->viewport_height,
        0.0f
    );
    shader->set_uniform("projection", proj);

    auto render_group = std::make_shared<RenderGroup>(shader, proj);
    this->render_groups[this->render_group_count] = render_group;
    this->render_group_count++;
    return render_group;
}
