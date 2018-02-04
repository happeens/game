#pragma once

#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <types.hpp>

struct Shader {
    GLuint id;
};

Shader load_shader(const char* vertex_path, const char* fragment_path);
void use_shader(Shader shader);

void set_uniform(Shader shader, const std::string& name, bool value);
void set_uniform(Shader shader, const std::string& name, i32 value);
void set_uniform(Shader shader, const std::string& name, f32 value);
void set_uniform(Shader shader, const std::string& name, glm::mat4 value);

