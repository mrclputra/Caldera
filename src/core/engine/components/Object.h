#pragma once

#include <memory>
#include <string>

#include "PointCloud.h"
#include "Transform.h"

class Object {
public:
    explicit Object(const std::string& name = "Object") : name(name) {}
    ~Object() = default;

    Transform transform;
    std::string name;
    std::shared_ptr<PointCloud> pointCloud;
};
