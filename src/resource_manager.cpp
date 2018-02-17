#include <resource_manager.hpp>

#include <stdio.h>
#include <sstream>

std::shared_ptr<Shader> ResourceManager::get_shader(const std::string& name) {
    if (this->shaders.count(name) > 0) return this->shaders[name];

    std::ostringstream path_buf;

    path_buf.str("");
    path_buf << "../src/shaders/" << name << ".vs.glsl";
    auto vertex_path = path_buf.str();

    path_buf.str("");
    path_buf << "../src/shaders/" << name << ".fs.glsl";
    auto fragment_path = path_buf.str();

    auto shader = std::make_shared<Shader>(vertex_path, fragment_path);
    this->shaders[name] = shader;
    return shader;
}

