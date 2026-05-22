#ifndef SCENE_H
#define SCENE_H

#include <memory>

#include "Camera.h"
#include "components/PointCloud.h"

class Scene {
public:
    Scene() = default;
    ~Scene() = default;

    std::vector<std::shared_ptr<PointCloud>> point_clouds;
    Camera camera;

    void addCloud(std::shared_ptr<PointCloud> pc);
    void removeCloud(const std::shared_ptr<PointCloud> &pc);
    // void update(float dt);
};

#endif // SCENE_H