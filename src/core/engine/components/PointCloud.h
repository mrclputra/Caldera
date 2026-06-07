#ifndef POINTCLOUD_H
#define POINTCLOUD_H

#include <QOpenGLFunctions_4_5_Core>
#include <glm/glm.hpp>
#include <vector>

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

#include "Transform.h"

class PointCloud {
public:
    struct Point {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec4 color;
    };

    PointCloud(
        QOpenGLFunctions_4_5_Core* f,
        const std::string& name,
        std::vector<Point> points
    );
    ~PointCloud();

    void upload();
    void render();

    Transform transform;
    std::string name;
    glm::vec3 bb_min{0.0f};
    glm::vec3 bb_max{0.0f};

private:
    QOpenGLFunctions_4_5_Core* f;
    std::vector<Point> points;

    GLuint VBO = 0;
    GLuint VAO = 0;
};

#endif // POINTCLOUD_H