#include "Camera.h"

#include <Qt>
#include <algorithm>
#include "glm/gtc/quaternion.hpp"
#include "spdlog/spdlog.h"

Camera::Camera(glm::vec3 position) {
    this->position = position;
    SPDLOG_INFO("camera created; {} * {}", viewport_width, viewport_height);
    updateVectors();
}

void Camera::updateVectors() {
    glm::vec3 v;
    v.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    v.y = sin(glm::radians(pitch));
    v.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    // https://www.songho.ca/opengl/gl_camera.html
    front = glm::normalize(v);
    right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    up = glm::normalize(glm::cross(right, front));
}

void Camera::update(float delta) {
    if (held_keys.count(Qt::Key_W)) moveForward(delta);
    if (held_keys.count(Qt::Key_S)) moveBackward(delta);
    if (held_keys.count(Qt::Key_A)) moveLeft(delta);
    if (held_keys.count(Qt::Key_D)) moveRight(delta);
    if (held_keys.count(Qt::Key_E)) moveUp(delta);
    if (held_keys.count(Qt::Key_Q)) moveDown(delta);
    updateVectors();
}

void Camera::onKeyPress(int key)  { held_keys.insert(key); }
void Camera::onKeyRelease(int key) { held_keys.erase(key); }
void Camera::onMouseMove(int dx, int dy) { rotate(static_cast<float>(dx), static_cast<float>(dy)); }
void Camera::onScroll(int delta) {
    constexpr float min_speed = 2.0f;
    constexpr float max_speed = 140.0f;
    constexpr float step = 4.0f;
    speed = std::clamp(speed + (delta > 0 ? step : -step), min_speed, max_speed);
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(
      glm::radians(fov),
      static_cast<float>(viewport_width) / static_cast<float>(viewport_height),
      0.1f,
      100000.0f
    );
}

void Camera::rotate(float x_offset, float y_offset) {
    yaw += x_offset * sensitivity;
    pitch -= y_offset * sensitivity;

    pitch = glm::clamp(pitch, -89.0f, 89.0f);
    updateVectors();
}

void Camera::moveForward(const float delta) {
    position += front * speed * delta;
}
void Camera::moveBackward(const float delta) {
    position -= front * speed * delta;
}
void Camera::moveRight(const float delta) {
    position += right * speed * delta;
}
void Camera::moveLeft(const float delta) {
    position -= right * speed * delta;
}
void Camera::moveUp(const float delta) {
    position += up * speed * 0.5f * delta;
}
void Camera::moveDown(const float delta) {
    position -= up * speed * 0.5f * delta;
}

void Camera::setViewport(int width, int height) {
    viewport_width = width;
    viewport_height = height;
}
