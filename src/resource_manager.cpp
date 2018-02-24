#include <resource_manager.hpp>

#include <stdio.h>
#include <sstream>

std::shared_ptr<Shader> ResourceManager::get_shader(const std::string& name) {
    if (this->shaders.count(name) > 0) return this->shaders[name];

    std::ostringstream path_buf;

    path_buf << "../src/shaders/" << name << ".vs.glsl";
    auto vertex_path = path_buf.str();

    path_buf.str("");
    path_buf << "../src/shaders/" << name << ".fs.glsl";
    auto fragment_path = path_buf.str();

    auto shader = std::make_shared<Shader>(vertex_path, fragment_path);
    this->shaders[name] = shader;
    return shader;
}

std::shared_ptr<Texture> ResourceManager::get_texture(const std::string& name) {
    if (this->textures.count(name) > 0) return this->textures[name];

    std::ostringstream path_buf;
    path_buf << "../assets/" << name;
    auto path = path_buf.str();

    auto texture = std::make_shared<Texture>(path);
    this->textures[name] = texture;
    return texture;
}

std::shared_ptr<Texture> ResourceManager::make_batch_texture(
    const std::string& name,
    const BatchInput& sprites
) {
    std::vector<std::string> batch = {};

    std::ostringstream path_buf;
    for (auto it : sprites.values) {
        path_buf << "../assets/" << it;
        batch.push_back(path_buf.str());
        path_buf.str("");
    }

    auto texture = std::make_shared<Texture>(batch);
    this->textures[name] = texture;
    return texture;
}

