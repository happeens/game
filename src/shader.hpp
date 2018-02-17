#ifndef __SHADER__
#define __SHADER__

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

    Shader(const std::string& vertex_path, const std::string& fragment_path);
    ~Shader();

    void use() const;

    void set_uniform(const std::string& name, bool value) const;
    void set_uniform(const std::string& name, i32 value) const;
    void set_uniform(const std::string& name, f32 value) const;
    void set_uniform(const std::string& name, glm::mat4 value) const;
};

#endif

