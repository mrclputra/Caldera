#include "Scene.h"

void Scene::update(float deltaTime) {

}

void Scene::addObject(std::shared_ptr<Object> object) {
    if (object) {
        objects.push_back(std::move(object));
    }
}

void Scene::removeObject(const std::shared_ptr<Object> &object) {
    auto it = std::remove(objects.begin(), objects.end(), object);
    if (it != objects.end()) {
        objects.erase(it, objects.end());
    }
}
