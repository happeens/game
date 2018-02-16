#include <stdio.h>

#include <render_group.hpp>
#include <GLFW/glfw3.h>
#include <types.hpp>

#define MAX_RENDER_GROUP_COUNT 10

struct RenderInfo {
    i32 viewport_width = 0;
    i32 viewport_height = 0;

    const GLubyte* renderer_string;
    const GLubyte* version_string;

    RenderGroup* render_groups[MAX_RENDER_GROUP_COUNT];
    u32 render_group_count = 0;
    glm::mat4 projection;

    RenderInfo(GLFWwindow* window);
    ~RenderInfo();

    void draw() const;

    // NOTE: consumes the given render group! maybe use a unique pointer?
    void push_render_group(RenderGroup* render_group);

    // TODO: how to communicate that this is a non owning pointer?
    RenderGroup* create_primitive_render_group();
};
