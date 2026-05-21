#pragma once
#include <memory>

#include "Camera.h"
#include "components/Object.h"

class Scene {
public:
    Scene() = default;
    ~Scene() = default;

    std::vector<std::shared_ptr<Object>> objects;
    Camera camera;

    void addObject(std::shared_ptr<Object> object);
    void removeObject(const std::shared_ptr<Object> &object);

    void update(float deltaTime);
};
