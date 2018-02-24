#pragma once

#include <memory>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <common.hpp>
#include <render_group.hpp>

#define MAX_RENDER_GROUP_COUNT 10

struct RenderContext {
    Viewport viewport;

    const GLubyte* renderer_string;
    const GLubyte* version_string;

    std::vector<std::shared_ptr<RenderGroup>> render_groups = {};

    RenderContext(GLFWwindow* window);
    ~RenderContext();

    void draw() const;

    void update_viewport(i32 width, i32 height);

    std::shared_ptr<RenderGroup> create_primitive_render_group();
    std::shared_ptr<RenderGroup> create_sprite_render_group(
        const std::string& texture_name
    );
    std::shared_ptr<RenderGroup> create_sprite_batch_render_group(
        const std::string& texture_name,
        const BatchInput& sprites
    );
};

