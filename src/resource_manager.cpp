#include <resource_manager.hpp>

std::shared_ptr<Shader> ResourceManager::get_shader(const std::string& name) {
    if (this->shaders.count(name) > 0) return this->shaders[name];

    std::ostringstream path_buf;

    path_buf.str("");
    path_buf << "../src/shaders/" << name << ".vs.glsl";
    std::string vertex_path = path_buf.str();

    path_buf.str("");
    path_buf << "../src/shaders/" << name << ".fs.glsl";
    std::string fragment_path = path_buf.str();

    auto shader = std::make_shared<Shader>(vertex_path, fragment_path);
    this->shaders[name] = shader;
    return shader;
}

