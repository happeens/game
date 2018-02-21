#include <stdio.h>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <common.hpp>
#include <render_info.hpp>

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

    glfwWindowHint(GLFW_RED_BITS, 4);
    glfwWindowHint(GLFW_BLUE_BITS, 4);
    glfwWindowHint(GLFW_GREEN_BITS, 4);
    glfwWindowHint(GLFW_ALPHA_BITS, 4);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "game", NULL, NULL);
    if (!window) {
        printf("window creation failed\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    auto render_info = new RenderInfo(window);

    auto sprite_group = render_info->create_sprite_render_group();
    auto primitive_group = render_info->create_primitive_render_group();

    ColoredRect rect1;
    rect1.x_min = 10;
    rect1.x_max = 100;
    rect1.y_min = 10;
    rect1.y_max = 100;
    primitive_group->push_rect(rect1);

    ColoredRect rect2;
    rect2.x_min = 100;
    rect2.x_max = 150;
    rect2.y_min = 110;
    rect2.y_max = 160;
    primitive_group->push_rect(rect2);

    ColoredRect rect3;
    rect3.x_min = 210;
    rect3.x_max = 250;
    rect3.y_min = 250;
    rect3.y_max = 290;
    primitive_group->push_rect(rect3);

    f32 last_time = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        f32 now = glfwGetTime();
        f32 elapsed = now - last_time;
        now = last_time;

        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render_info->draw();

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
