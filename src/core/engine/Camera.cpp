#include "Camera.h"

#include "glm/gtc/quaternion.hpp"
#include "spdlog/spdlog.h"

Camera::Camera(glm::vec3 position) {
    this->position = position;
    spdlog::info("camera created; {} * ", viewportWidth, viewportHeight);
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
      static_cast<float>(viewportWidth) / static_cast<float>(viewportHeight),
      0.1f,
      100.0f
    );
}

void Camera::rotate(float xOffset, float yOffset) {
    yaw += xOffset * sensitivity;
    pitch -= yOffset * sensitivity;

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
    position += up * speed * delta;
}
void Camera::moveDown(const float delta) {
    position -= up * speed * delta;
}

void Camera::setViewport(int width, int height) {
    viewportWidth = width;
    viewportHeight = height;
}
