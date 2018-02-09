#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <types.hpp>
#include <shader.hpp>
#include <render_group.hpp>
#include <render_info.hpp>

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

static void error_callback(i32 error, const char* description) {
    printf("an error has occured: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
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

    glewExperimental = GL_TRUE;
    glewInit();

    auto render_info = new RenderInfo(window);

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
    push_rect(render_group, 50, 200, 50, 200);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render_info->draw();

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
