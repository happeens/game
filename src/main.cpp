#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <types.hpp>
#include <shader.hpp>
#include <render_group.hpp>

#define MAX_RENDER_GROUP_COUNT 10

struct RenderInfo {
    i32 viewport_width;
    i32 viewport_height;

    RenderGroup* render_groups[MAX_RENDER_GROUP_COUNT];
    u32 render_group_count;
    glm::mat4 projection;
};

static void terminate(RenderInfo* render_info) {
    for (u32 i = 0; i < render_info->render_group_count; i++) {
        delete render_info->render_groups[i];
    }

    delete render_info;
}

static void push_rect(
    RenderGroup* render_group,
    i32 x_min, i32 x_max,
    i32 y_min, i32 y_max
) {
    Rect rect = {};
    rect.x_min = x_min;
    rect.x_max = x_max;
    rect.y_min = y_min;
    rect.y_max = y_max;

    render_group->rects[render_group->rect_count] = rect;
    render_group->rect_count++;
}

static void draw(RenderInfo* render_info) {
    for (u32 i = 0; i < render_info->render_group_count; i++) {
        render_info->render_groups[i]->draw();
    }
}

static void error_callback(i32 error, const char* description) {
    printf("an error has occured: %s\n", description);
}

static void key_callback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods) {
    printf("key pressed: %d", key);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void framebuffer_size_callback(GLFWwindow* window, i32 width, i32 height) {
    printf("resized to %d, %d!\n", width, height);
    glViewport(0, 0, width, height);
}

void get_gl_error() {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("there was an error\n");

        if (error == GL_INVALID_ENUM)
            printf("invalid enum\n");

        if (error == GL_INVALID_VALUE)
            printf("invalid value\n");

        if (error == GL_INVALID_OPERATION)
            printf("invalid operation\n");

        if (error == GL_INVALID_FRAMEBUFFER_OPERATION)
            printf("invalid framebuffer operation\n");

        if (error == GL_OUT_OF_MEMORY)
            printf("out of memory\n");
    }
}

i32 main(i32 argc, char *argv[]) {
    if (!glfwInit()) {
        printf("glfw init failed\n");
        return -1;
    }

    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(1000, 1000, "game", NULL, NULL);
    if (!window) {
        printf("window creation failed\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    const GLubyte* renderer_string = glGetString(GL_RENDERER);
    const GLubyte* version_string = glGetString(GL_VERSION);

    printf("renderer: %s, opengl version: %s\n", renderer_string, version_string);

    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    auto render_info = new RenderInfo;
    render_info->render_group_count = 0;

    glfwGetFramebufferSize(
        window,
        &render_info->viewport_width,
        &render_info->viewport_height
    );

    glViewport(0, 0, render_info->viewport_width, render_info->viewport_height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Shader shader(
        "../src/shaders/default.vs.glsl",
        "../src/shaders/default.fs.glsl"
    );

    shader.use();

    glm::mat4 proj = glm::ortho(
        0.0f,
        (f32) render_info->viewport_width,
        (f32) render_info->viewport_height,
        0.0f
    );
    shader.set_uniform("projection", proj);

    auto render_group = new RenderGroup;

    render_info->render_groups[0] = render_group;
    render_info->render_group_count++;
    push_rect(render_group, 50, 200, 10, 500);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        draw(render_info);

        glfwSwapBuffers(window);
    }

    terminate(render_info);
    glfwTerminate();
    return 0;
}
