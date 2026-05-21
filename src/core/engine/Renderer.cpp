#include "Renderer.h"
#include <Qt>
#include <spdlog/spdlog.h>

Renderer::Renderer(
    QOpenGLFunctions_4_5_Core* f,
    const int width,
    const int height
) {
    this->f = f;
    this->width = width;
    this->height = height;
}

void Renderer::initialize() {
    shader = std::make_shared<Shader>(
        f,
        std::string(SHADER_DIR) + "pc.vert",
        std::string(SHADER_DIR) + "pc.frag"
    );

    f->glEnable(GL_PROGRAM_POINT_SIZE);

    scene.camera.position = glm::vec3(0.0f, 0.0f, 3.0f);
    scene.camera.setViewport(width, height);
}

void Renderer::render(float deltaTime) {
    if (held_keys.count(Qt::Key_W)) scene.camera.moveForward(deltaTime);
    if (held_keys.count(Qt::Key_S)) scene.camera.moveBackward(deltaTime);
    if (held_keys.count(Qt::Key_A)) scene.camera.moveLeft(deltaTime);
    if (held_keys.count(Qt::Key_D)) scene.camera.moveRight(deltaTime);
    if (held_keys.count(Qt::Key_E)) scene.camera.moveUp(deltaTime);
    if (held_keys.count(Qt::Key_Q)) scene.camera.moveDown(deltaTime);
    scene.camera.update();

    glm::mat4 view = scene.camera.getViewMatrix();
    glm::mat4 proj = scene.camera.getProjectionMatrix();

    shader->bind();
    shader->setMat4("view", view);
    shader->setMat4("proj", proj);

    for (const auto& obj : scene.objects) {
        if (!obj->pointCloud) continue;
        shader->setMat4("model", obj->transform.getTransformationMatrix());
        shader->setFloat("pointSize", obj->pointCloud->pointSize);
        obj->pointCloud->render();
    }
}

void Renderer::resize(int width, int height) {
    spdlog::info("renderer resize: {},{}", width, height);
    width = width;
    height = height;
    scene.camera.setViewport(width, height);
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
