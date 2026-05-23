#include "Renderer.h"
#include <Qt>
#include <algorithm>
#include <spdlog/spdlog.h>

#include "utilities/Loader.h"

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
    // this is the point cloud shader
    shader = std::make_shared<Shader>(
        f,
        std::string(SHADER_DIR) + "cloud.vert",
        std::string(SHADER_DIR) + "cloud.frag"
    );

    auto cloud = loadPointCloud(f, "D:/datasets/boardwalk_in_the_forest_-_point_cloud/scene.gltf");
    // auto cloud = ModelLoader::load(f, "D:/datasets/ClaustroVelezBlanco.ply");
    // auto cloud = ModelLoader::load(f, "D:/datasets/ce4c13a6d4fc44318477608e45341e7e.ply");
    // auto cloud = loadPointCloud(f, "D:/datasets/anglefit0/fully_aligned_fullres.ply"); // no rotate needed
    if (cloud) {
        cloud->transform.rotate(glm::vec3(-90.0f, 0.0f, 0.0f));
        scene.addCloud(cloud);
    } else {
        spdlog::error("unable to load cloud");
    }

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

    shader->setFloat("pointSize", point_size);

    for (const auto& pc : scene.point_clouds) {
        glm::mat4 model = pc->transform.getTransformationMatrix();
        shader->setMat4("model", model);
        pc->render();
        gizmos.drawBoundingBox({pc->bb_min, pc->bb_max}, model, view, proj, {1.0f, 1.0f, 1.0f, 1.0f});
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

void Renderer::onScroll(int delta) {
    constexpr float min_speed = 2.0f;
    constexpr float max_speed = 30.0f;
    constexpr float step = 1.0f;
    scene.camera.speed = std::clamp(
        scene.camera.speed + (delta > 0 ? step : -step),
        min_speed, max_speed
    );
    spdlog::info("camera speed: {:.1f}", scene.camera.speed);
}
