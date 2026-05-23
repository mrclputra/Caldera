#include "Camera.h"

#include "glm/gtc/quaternion.hpp"
#include "spdlog/spdlog.h"

Camera::Camera(glm::vec3 position) {
    this->position = position;
    SPDLOG_INFO("camera created; {} * {}", viewport_width, viewport_height);
    update();
}

void Camera::update() {
    glm::vec3 v;
    v.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    v.y = sin(glm::radians(pitch));
    v.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    // https://www.songho.ca/opengl/gl_camera.html
    front = glm::normalize(v);
    right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f))); // modify y if we need to implement roll later
    up = glm::normalize(glm::cross(right, front));
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
    update();
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
