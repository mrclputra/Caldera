#include "Renderer.h"
#include <Qt>
#include <spdlog/spdlog.h>

#include "utilities/ModelLoader.h"

Renderer::Renderer(
    QOpenGLFunctions_4_5_Core* f,
    const int w,
    const int h
) : gizmos(f) {
    this->f = f;
    this->width = w;
    this->height = h;
}

void Renderer::initialize() {
    shader = std::make_shared<Shader>(
        f,
        std::string(SHADER_DIR) + "pc.vert",
        std::string(SHADER_DIR) + "pc.frag"
    );

    auto cloud = ModelLoader::load(f, "D:/datasets/boardwalk_in_the_forest_-_point_cloud/scene.gltf");
    // auto cloud = ModelLoader::load(f, "D:/datasets/ClaustroVelezBlanco.ply");

    // auto cloud = ModelLoader::load(f, "D:/datasets/ce4c13a6d4fc44318477608e45341e7e.ply");
    // cloud->transform.rotate(glm::vec3(0.0f, 180.0f, 0.0f));

    cloud->transform.rotate(glm::vec3(-90.0f, 0.0f, 0.0f));
    if (cloud) scene.addObject(cloud);

    gizmos.initialize();

    f->glEnable(GL_PROGRAM_POINT_SIZE);

    scene.camera.position = glm::vec3(0.0f, 0.0f, 30.0f);
    scene.camera.setViewport(width, height);
}

void Renderer::render(float delta_time) {
    if (held_keys.count(Qt::Key_W)) scene.camera.moveForward(delta_time);
    if (held_keys.count(Qt::Key_S)) scene.camera.moveBackward(delta_time);
    if (held_keys.count(Qt::Key_A)) scene.camera.moveLeft(delta_time);
    if (held_keys.count(Qt::Key_D)) scene.camera.moveRight(delta_time);
    if (held_keys.count(Qt::Key_E)) scene.camera.moveUp(delta_time);
    if (held_keys.count(Qt::Key_Q)) scene.camera.moveDown(delta_time);
    scene.camera.update();

    glm::mat4 view = scene.camera.getViewMatrix();
    glm::mat4 proj = scene.camera.getProjectionMatrix();

    shader->bind();
    shader->setMat4("view", view);
    shader->setMat4("proj", proj);

    for (const auto& obj : scene.objects) {
        if (!obj->point_cloud) continue;
        glm::mat4 model = obj->transform.getTransformationMatrix();
        shader->setMat4("model", model);
        shader->setFloat("pointSize", obj->point_cloud->point_size);
        obj->point_cloud->render();
        gizmos.drawBoundingBox(obj->point_cloud->bounding_box, model, view, proj, {1.0f, 1.0f, 1.0f, 1.0f});
    }
}

void Renderer::resize(int w, int h) {
    spdlog::info("renderer resize: {},{}", w, h);
    this->width = w;
    this->height = h;
    scene.camera.setViewport(w, h);
}

void Renderer::onKeyPress(int key) {
    held_keys.insert(key);
}

void Renderer::onKeyRelease(int key) {
    held_keys.erase(key);
}

void Renderer::onMouseMove(int dx, int dy) {
    scene.camera.rotate(static_cast<float>(dx), static_cast<float>(dy));
}
