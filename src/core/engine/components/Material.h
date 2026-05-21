#pragma once

#include <glm/glm.hpp>

class Material {
public:
    Material() = default;
    ~Material() = default;

    // parameters
    glm::vec4 albedo = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // rgba
    // todo: consider implementing the gltf convention
};