#include "Scene.h"

void Scene::addCloud(std::shared_ptr<PointCloud> pc) {
    if (pc) {
        point_clouds.push_back(std::move(pc));
    }
}
void Scene::removeCloud(const std::shared_ptr<PointCloud> &pc) {
    if (auto it = std::remove(point_clouds.begin(), point_clouds.end(), pc); it != point_clouds.end()) {
        point_clouds.erase(it, point_clouds.end());
    }
}

// void Scene::update(float dt) {}
