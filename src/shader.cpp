#include <shader.hpp>

Shader load_shader(const GLchar* vertex_path, const GLchar* fragment_path) {
    Shader result = {};

    std::string vertex_src;
    std::string fragment_src;

    std::ifstream vertex_file;
    std::ifstream fragment_file;

    vertex_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragment_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        std::stringstream vertex_stream, fragment_stream;

        vertex_file.open(vertex_path);
        vertex_stream << vertex_file.rdbuf();
        vertex_file.close();

        fragment_file.open(fragment_path);
        fragment_stream << fragment_file.rdbuf();
        fragment_file.close();

        vertex_src = vertex_stream.str();
        fragment_src = fragment_stream.str();

    } catch (std::ifstream::failure e) {
        printf("error: shader files could not be read\n");
        return result;
    }

    GLuint vertex, fragment;
    i32 success;
    char compile_log[512];

    auto vertex_src_c = vertex_src.c_str();
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_src_c, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, compile_log);
        printf("error: vertex compilation failed\n%s\n", compile_log);
        return result;
    }

    auto fragment_src_c = fragment_src.c_str();
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_src_c, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, compile_log);
        printf("error: fragment compilation failed\n%s\n", compile_log);
        return result;
    }

    result.id = glCreateProgram();
    glAttachShader(result.id, vertex);
    glAttachShader(result.id, fragment);
    glLinkProgram(result.id);

    glGetProgramiv(result.id, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(result.id, 512, NULL, compile_log);
        printf("error: shader linking failed\n%s\n", compile_log);
        return result;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return result;
}

void use_shader(Shader shader) {
    glUseProgram(shader.id);
}

void set_uniform(Shader shader, const std::string& name, bool value) {
    glUniform1i(glGetUniformLocation(shader.id, name.c_str()), (int) value);
}

void set_uniform(Shader shader, const std::string& name, i32 value) {
    glUniform1i(glGetUniformLocation(shader.id, name.c_str()), value);
}

void set_uniform(Shader shader, const std::string& name, f32 value) {
    glUniform1f(glGetUniformLocation(shader.id, name.c_str()), value);
}

void set_uniform(Shader shader, const std::string& name, glm::mat4 value) {
    glUniformMatrix4fv(
        glGetUniformLocation(shader.id, name.c_str()),
        1,
        GL_FALSE,
        glm::value_ptr(value)
    );
}

