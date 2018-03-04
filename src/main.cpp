#include <stdio.h>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <common.hpp>
#include <render_context.hpp>
#include <texture.hpp>

#include <text.hpp>

struct Context {
    RenderContext* render_context;
};

static Context* get_context(GLFWwindow* window) {
    return static_cast<Context*>(glfwGetWindowUserPointer(window));
}

static void error_callback(i32 error, const char* description) {
    printf("an error has occured: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    printf("key pressed: %d\n", key);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void framebuffer_size_callback(GLFWwindow* window, i32 width, i32 height) {
    auto context = get_context(window);
    context->render_context->update_viewport(width, height);
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

    glfwWindowHint(GLFW_DECORATED, false);

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

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    auto context = new Context;
    auto render_context = new RenderContext(window);
    context->render_context = render_context;

    glfwSetWindowUserPointer(window, (void*) context);

    auto batch_group = render_context->create_sprite_batch_render_group(
        "test-batch",
        BatchInput{
            "sprite0.png",
            "sprite1.png",
            "sprite2.png"
        }
    );

    auto sprite0 = TexturedRect(Position(10.0f, 10.0f), Size(200.0f, 200.0f), 0);
    auto sprite1 = TexturedRect(Position(200.0f, 10.0f), Size(200.0f, 200.0f), 1);
    auto sprite2 = TexturedRect(Position(400.0f, 10.0f), Size(200.0f, 200.0f), 2);
    batch_group->push_rect(sprite0);
    batch_group->push_rect(sprite1);
    batch_group->push_rect(sprite2);

    auto font_group = render_context->create_text_render_group();
    auto c_a = CharacterRect(
        Position(10.0f, 400.0f), 3.0f,
        'a', Color(1.0f, 1.0f, 1.0f, 1.0f)
    );
    auto c_b = CharacterRect(
        Position(110.0f, 400.0f), 3.0f,
        'b', Color(1.0f, 1.0f, 1.0f, 1.0f)
    );
    auto c_c = CharacterRect(
        Position(220.0f, 400.0f), 3.0f,
        'c', Color(1.0f, 1.0f, 1.0f, 1.0f)
    );
    auto c_d = CharacterRect(
        Position(330.0f, 400.0f), 3.0f,
        'd', Color(1.0f, 1.0f, 1.0f, 1.0f)
    );

    font_group->push_rect(c_a);
    font_group->push_rect(c_b);
    font_group->push_rect(c_c);
    font_group->push_rect(c_d);

    f32 last_time = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        f32 now = glfwGetTime();
        f32 elapsed = now - last_time;
        now = last_time;

        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render_context->draw();

        glfwSwapBuffers(window);
    }

    delete (render_context);
    glfwTerminate();
    return 0;
}
