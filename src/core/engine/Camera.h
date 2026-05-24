#ifndef CAMERA_H
#define CAMERA_H

#include <set>
#include "glm/glm.hpp"

static float PIXEL_TO_RAD = 0.01f;

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front, up, right;

    float yaw = -90.0f;
    float pitch = 0.0f;

    float sensitivity = 0.10f;
    float speed = 10.0f;
    float fov = 32.0f;

    explicit Camera(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f)
    );

    void update(float delta);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    void rotate(float x_offset, float y_offset);
    void moveForward(float delta);
    void moveBackward(float delta);
    void moveRight(float delta);
    void moveLeft(float delta);
    void moveUp(float delta);
    void moveDown(float delta);

    void onKeyPress(int key);
    void onKeyRelease(int key);
    void onMouseMove(int dx, int dy);
    void onScroll(int delta);

    void setViewport(int width, int height);

private:
    void updateVectors();

    std::set<int> held_keys;
    glm::vec3 world_up;
    int viewport_width = 800;
    int viewport_height = 600;
};

#endif // CAMERA_H