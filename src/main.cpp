#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void error_callback(int error, const char* description) {
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

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // setup done at this point

    // compile shaders

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_source, NULL);
    glCompileShader(vertex_shader);

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_source, NULL);
    glCompileShader(fragment_shader);

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);

    GLint vertex_status;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vertex_status);
    printf("vertex compiled: %d\n", vertex_status == GL_TRUE);

    GLint fragment_status;
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &fragment_status);
    printf("fragment compiled: %d\n", fragment_status == GL_TRUE);

    glLinkProgram(shader_program);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    float vertices[] = {
        10.0f, 10.0f, // top left
        100.0f, 10.0f, // top right
        10.0f, 100.0f, // bottom left
        100.0f, 100.0f, // bottom right
    };

    GLuint elements[] = {
        0, 1, 2,
        1, 2, 3,
        4, 5, 6,
        5, 6, 7
    };

    glUseProgram(shader_program);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    GLint attrib_pos = glGetAttribLocation(shader_program, "position");
    glEnableVertexAttribArray(attrib_pos);
    glVertexAttribPointer(attrib_pos, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

    glm::mat4 proj_matrix = glm::ortho(0.0f, (float) width, (float) height, 0.0f);
    GLint proj_loc = glGetUniformLocation(shader_program, "projection");
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(proj_matrix));

    get_gl_error();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
