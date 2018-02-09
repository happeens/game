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

class Shader {
public:
    GLuint id;

    Shader(const char* vertex_path, const char* fragment_path);
    ~Shader();

    void use() const;

    void set_uniform(const std::string& name, bool value) const;
    void set_uniform(const std::string& name, i32 value) const;
    void set_uniform(const std::string& name, f32 value) const;
    void set_uniform(const std::string& name, glm::mat4 value) const;
};


