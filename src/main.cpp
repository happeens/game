#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define MAX_RECT_COUNT 1024

struct Rect {
    float x_min;
    float x_max;
    float y_min;
    float y_max;
};

struct RenderBuffer {
    Rect rects[MAX_RECT_COUNT];
    int rect_count;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
};

struct RenderInfo {
    int viewport_width;
    int viewport_height;

    GLuint shader_program;
    RenderBuffer render_buffer;
    glm::mat4 projection;
};

static void clear_buffer(RenderBuffer* render_buffer) {
    render_buffer->rect_count = 0;
}

static void push_rect(RenderBuffer* render_buffer, int x_min, int x_max, int y_min, int y_max) {
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

    float vertices[MAX_RECT_COUNT * 8] = {};
    GLuint elements[MAX_RECT_COUNT * 6] = {};

    for (int i = 0; i < render_info->render_buffer.rect_count; i++) {
        int vert_index = i * 8;
        vertices[vert_index] = (float) render_info->render_buffer.rects[i].x_min;
        vertices[vert_index + 1] = (float) render_info->render_buffer.rects[i].y_min;

        vertices[vert_index + 2] = (float) render_info->render_buffer.rects[i].x_max;
        vertices[vert_index + 3] = (float) render_info->render_buffer.rects[i].y_min;

        vertices[vert_index + 4] = (float) render_info->render_buffer.rects[i].x_min;
        vertices[vert_index + 5] = (float) render_info->render_buffer.rects[i].y_max;

        vertices[vert_index + 6] = (float) render_info->render_buffer.rects[i].x_max;
        vertices[vert_index + 7] = (float) render_info->render_buffer.rects[i].y_max;

        int elem_index = i * 6;
        int elem = i * 4;
        elements[elem_index] = elem;
        elements[elem_index + 1] = elem + 1;
        elements[elem_index + 2] = elem + 2;
        elements[elem_index + 3] = elem + 1;
        elements[elem_index + 4] = elem + 2;
        elements[elem_index + 5] = elem + 3;
    }

    // printf("\nverts: ");
    // for (int i = 0; i < render_info->render_buffer.rect_count * 8; i++) {
    //     printf("%f, ", vertices[i]);
    // }

    // printf("\nelems: ");
    // for (int i = 0; i < render_info->render_buffer.rect_count * 6; i++) {
    //     printf("%d, ", elements[i]);
    // }

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

static void error_callback(int error, const char* description) {
    printf("an error has occured: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    printf("key pressed: %d", key);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
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

const char* vertex_source = R"glsl(
#version 330 core

in vec2 position;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(position, 0.0, 1.0);
}
)glsl";

const char* fragment_source = R"glsl(
#version 330 core

out vec4 out_color;

void main() {
    out_color = vec4(1.0, 1.0, 1.0, 1.0);
}
)glsl";

int main(int argc, char *argv[]) {
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

    // compile shaders

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_source, NULL);
    glCompileShader(vertex_shader);

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_source, NULL);
    glCompileShader(fragment_shader);

    render_info->shader_program = glCreateProgram();
    glAttachShader(render_info->shader_program, vertex_shader);
    glAttachShader(render_info->shader_program, fragment_shader);

    GLint vertex_status;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vertex_status);
    printf("vertex compiled: %d\n", vertex_status == GL_TRUE);

    GLint fragment_status;
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &fragment_status);
    printf("fragment compiled: %d\n", fragment_status == GL_TRUE);

    glLinkProgram(render_info->shader_program);

    render_info->render_buffer.vao;
    glGenVertexArrays(1, &render_info->render_buffer.vao);
    glBindVertexArray(render_info->render_buffer.vao);

    for (int i = 0; i < 10; i++) {
        push_rect(&render_info->render_buffer, i * 60, (i * 60) + 50, 10, 50);
    }

    glUseProgram(render_info->shader_program);

    glGenBuffers(1, &render_info->render_buffer.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, render_info->render_buffer.vbo);

    glGenBuffers(1, &render_info->render_buffer.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_info->render_buffer.ebo);

    GLint attrib_pos = glGetAttribLocation(render_info->shader_program, "position");
    glEnableVertexAttribArray(attrib_pos);
    glVertexAttribPointer(attrib_pos, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

    glm::mat4 proj_matrix = glm::ortho(0.0f, (float) render_info->viewport_width, (float) render_info->viewport_height, 0.0f);
    GLint proj_loc = glGetUniformLocation(render_info->shader_program, "projection");
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(proj_matrix));

    get_gl_error();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        draw(render_info);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
