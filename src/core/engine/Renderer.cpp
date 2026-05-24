#include "Renderer.h"
#include <spdlog/spdlog.h>

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
    pc_shader = std::make_shared<Shader>(
        f,
        std::string(SHADER_DIR) + "cloud.vert",
        std::string(SHADER_DIR) + "cloud.frag"
    );
    gz_shader = std::make_shared<Shader>(
        f,
        std::string(SHADER_DIR) + "gizmo.vert",
        std::string(SHADER_DIR) + "gizmo.frag"
    );

    gizmos.initialize(gz_shader.get());

    f->glEnable(GL_PROGRAM_POINT_SIZE);

    scene.camera.position = glm::vec3(0.0f, 0.0f, 30.0f);
    scene.camera.setViewport(width, height);
}

void Renderer::render(float delta_time) {
    scene.camera.update(delta_time);

    glm::mat4 view = scene.camera.getViewMatrix();
    glm::mat4 proj = scene.camera.getProjectionMatrix();

    pc_shader->bind();
    pc_shader->setMat4("view", view);
    pc_shader->setMat4("proj", proj);
    pc_shader->setFloat("pointSize", point_size);

    for (const auto& pc : scene.point_clouds) {
        glm::mat4 model = pc->transform.getTransformationMatrix();
        pc_shader->setMat4("model", model);
        pc->render();
        gizmos.drawBoundingBox({pc->bb_min, pc->bb_max}, model, view, proj, {1.0f, 1.0f, 1.0f, 1.0f});
    }
}

void Renderer::resize(int w, int h) {
    this->width = w;
    this->height = h;
    scene.camera.setViewport(w, h);
}

void Renderer::onKeyPress(int key) { scene.camera.onKeyPress(key); }
void Renderer::onKeyRelease(int key) { scene.camera.onKeyRelease(key); }
void Renderer::onMouseMove(int dx, int dy) { scene.camera.onMouseMove(dx, dy); }
void Renderer::onScroll(int delta) { scene.camera.onScroll(delta); }
