#ifndef __RESOURCE_MANAGER__
#define __RESOURCE_MANAGER__

#include <stdio.h>
#include <unordered_map>
#include <memory>
#include <string>
#include <sstream>

// #include <texture.hpp>
#include <shader.hpp>
#include <types.hpp>

class ResourceManager {
public:
    static ResourceManager& get_instance() {
        static ResourceManager instance;
        return instance;
    }

    ResourceManager(ResourceManager const&) = delete;
    void operator =(ResourceManager const&) = delete;

    std::shared_ptr<Shader> get_shader(const std::string& name);

private:
    ResourceManager() {}

    std::unordered_map<std::string, std::shared_ptr<Shader>> shaders = {};
    // std::unordered_map<std::string, std::shared_ptr<Texture>> textures = {};
};

#endif

