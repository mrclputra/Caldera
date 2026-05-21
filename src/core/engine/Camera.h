#pragma once

#include <QOpenGLFunctions_4_5_Core>
#include "glm/glm.hpp"

static float PIXEL_TO_RAD = 0.01f;

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front, up, right; // needed for view matrix

    float yaw = -90.0f;
    float pitch = 0.0f;

    float sensitivity = 0.09f;
    float speed = 5.0f;
    float fov = 36.0f;

    Camera(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f)
        // glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f)
    );

    void update(); // this should be called every frame

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    void rotate(float xOffset, float yOffset);
    void moveForward(float delta);
    void moveBackward(float delta);
    void moveRight(float delta);
    void moveLeft(float delta);
    void moveUp(float delta);
    void moveDown(float delta);

    // call this when viewport dimensions change
    void setViewport(int width, int height);

private:
    glm::vec3 worldUp;
    int viewportWidth = 800;
    int viewportHeight = 600;
};