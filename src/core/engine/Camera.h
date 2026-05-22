#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"

static float PIXEL_TO_RAD = 0.01f;

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front, up, right; // needed for view matrix

    float yaw = -90.0f;
    float pitch = 0.0f;

    float sensitivity = 0.10f;
    float speed = 9.0f;
    float fov = 32.0f;

    explicit Camera(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f)
        // glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f) // this would be needed if we implement roll
    );

    void update(); // this should be called every frame

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    void rotate(float x_offset, float y_offset);
    void moveForward(float delta);
    void moveBackward(float delta);
    void moveRight(float delta);
    void moveLeft(float delta);
    void moveUp(float delta);
    void moveDown(float delta);

    // call this when viewport dimensions change
    void setViewport(int width, int height);

private:
    glm::vec3 world_up;
    int viewport_width = 800;
    int viewport_height = 600;
};

#endif // CAMERA_H