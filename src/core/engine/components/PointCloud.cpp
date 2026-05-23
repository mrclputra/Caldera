#include "PointCloud.h"

#include <spdlog/spdlog.h>

PointCloud::PointCloud(
    QOpenGLFunctions_4_5_Core *f,
    const std::string& name,
    std::vector<Point> points
) {
    this->f = f;
    this->name = name;
    this->points = std::move(points);
    upload();
}

PointCloud::~PointCloud() {
    if (VBO) f->glDeleteBuffers(1, &VBO);
    if (VAO) f->glDeleteVertexArrays(1, &VAO);
}

void PointCloud::upload() {
    if (VAO) {
        SPDLOG_ERROR("VAO already exists, cannot overwrite existing point cloud data.");
        return;
    }

    f->glGenVertexArrays(1, &VAO);
    f->glGenBuffers(1, &VBO);

    f->glBindVertexArray(VAO);

    f->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    f->glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(Point), points.data(), GL_STATIC_DRAW);

    f->glEnableVertexAttribArray(0); // position
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point), reinterpret_cast<void*>(offsetof(Point, position)));

    f->glEnableVertexAttribArray(1); // normal
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Point), reinterpret_cast<void*>(offsetof(Point, normal)));

    f->glEnableVertexAttribArray(2); // color
    f->glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Point), reinterpret_cast<void*>(offsetof(Point, color)));

    f->glBindVertexArray(0);

    bb_min = bb_max = points[0].position;
    for (const auto& p : points) {
        bb_min = glm::min(bb_min, p.position);
        bb_max = glm::max(bb_max, p.position);
    }
}

void PointCloud::render() {
    if (points.empty()) {
        SPDLOG_ERROR("point cloud has no points to render");
        return;
    }

    f->glBindVertexArray(VAO);
    f->glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(points.size()));
    f->glBindVertexArray(0);
}
