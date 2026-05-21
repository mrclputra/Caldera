#pragma once

#include <QOpenGLFunctions_4_5_Core>
#include <glm/glm.hpp>
#include <vector>

class PointCloud {
public:
    struct Point {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec4 color;
    };

    PointCloud(
        QOpenGLFunctions_4_5_Core* f,
        std::vector<Point> points
    );
    ~PointCloud();

    void upload();
    void render();

    float pointSize = 2.0f;

private:
    QOpenGLFunctions_4_5_Core* f;
    std::vector<Point> points;

    GLuint VBO = 0;
    GLuint VAO = 0;
};