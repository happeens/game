#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <types.hpp>
#include <shader.hpp>

#define MAX_RECT_COUNT 1024

struct Rect {
    f32 x_min;
    f32 x_max;
    f32 y_min;
    f32 y_max;
};

struct RenderBuffer {
    Rect rects[MAX_RECT_COUNT];
    u32 rect_count;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
};

struct RenderInfo {
    i32 viewport_width;
    i32 viewport_height;

    Shader shader;
    RenderBuffer render_buffer;
    glm::mat4 projection;
};

static void clear_buffer(RenderBuffer* render_buffer) {
    render_buffer->rect_count = 0;
}

static void push_rect(RenderBuffer* render_buffer, i32 x_min, i32 x_max, i32 y_min, i32 y_max) {
    Rect rect = {};
    rect.x_min = x_min;
    rect.x_max = x_max;
    rect.y_min = y_min;
    rect.y_max = y_max;

    render_buffer->rects[render_buffer->rect_count] = rect;
    render_buffer->rect_count++;
}

static void draw(RenderInfo* render_info) {
    glBindVertexArray(render_info->render_buffer.vao);

    f32 vertices[MAX_RECT_COUNT * 8] = {};
    GLuint elements[MAX_RECT_COUNT * 6] = {};

    for (u32 i = 0; i < render_info->render_buffer.rect_count; i++) {
        u32 vert_index = i * 8;
        vertices[vert_index] = (f32) render_info->render_buffer.rects[i].x_min;
        vertices[vert_index + 1] = (f32) render_info->render_buffer.rects[i].y_min;

        vertices[vert_index + 2] = (f32) render_info->render_buffer.rects[i].x_max;
        vertices[vert_index + 3] = (f32) render_info->render_buffer.rects[i].y_min;

        vertices[vert_index + 4] = (f32) render_info->render_buffer.rects[i].x_min;
        vertices[vert_index + 5] = (f32) render_info->render_buffer.rects[i].y_max;

        vertices[vert_index + 6] = (f32) render_info->render_buffer.rects[i].x_max;
        vertices[vert_index + 7] = (f32) render_info->render_buffer.rects[i].y_max;

        u32 elem_index = i * 6;
        u32 elem = i * 4;
        elements[elem_index] = elem;
        elements[elem_index + 1] = elem + 1;
        elements[elem_index + 2] = elem + 2;
        elements[elem_index + 3] = elem + 1;
        elements[elem_index + 4] = elem + 2;
        elements[elem_index + 5] = elem + 3;
    }

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_DYNAMIC_DRAW);

    glDrawElements(
        GL_TRIANGLES,
        render_info->render_buffer.rect_count * 6,
        GL_UNSIGNED_INT,
        0
    );

    glBindVertexArray(0);
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

    GLFWwindow* window = glfwCreateWindow(640, 480, "game", NULL, NULL);
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

    RenderInfo* render_info = new RenderInfo;

    glfwGetFramebufferSize(
        window,
        &render_info->viewport_width,
        &render_info->viewport_height
    );

    glViewport(0, 0, render_info->viewport_width, render_info->viewport_height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // setup done at this point

    render_info->shader = load_shader(
        "../src/shaders/default.vs.glsl",
        "../src/shaders/default.fs.glsl"
    );

    glGenVertexArrays(1, &render_info->render_buffer.vao);
    glBindVertexArray(render_info->render_buffer.vao);

    push_rect(&render_info->render_buffer, 50, 200, 10, 500);

    use_shader(render_info->shader);

    glGenBuffers(1, &render_info->render_buffer.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, render_info->render_buffer.vbo);

    glGenBuffers(1, &render_info->render_buffer.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_info->render_buffer.ebo);

    GLint attrib_pos = glGetAttribLocation(render_info->shader.id, "position");
    glEnableVertexAttribArray(attrib_pos);
    glVertexAttribPointer(attrib_pos, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

    glm::mat4 proj = glm::ortho(
        0.0f,
        (f32) render_info->viewport_width,
        (f32) render_info->viewport_height,
        0.0f
    );

    set_uniform(render_info->shader, "projection", proj);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        draw(render_info);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
