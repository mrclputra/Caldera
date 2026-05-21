#include "PointCloud.h"

#include <spdlog/spdlog.h>

PointCloud::PointCloud(QOpenGLFunctions_4_5_Core *f, std::vector<Point> points) {
    this->f = f;
    this->points = std::move(points);
    upload();
}

PointCloud::~PointCloud() {
    if (VBO) f->glDeleteBuffers(1, &VBO);
    if (VAO) f->glDeleteVertexArrays(1, &VAO);
}

void PointCloud::upload() {
    if (VAO) {
        spdlog::error("VAO already exists, cannot overwrite existing point cloud data.");
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

    bounding_box.min = bounding_box.max = points[0].position;
    for (const auto& p : points) {
        bounding_box.min = glm::min(bounding_box.min, p.position);
        bounding_box.max = glm::max(bounding_box.max, p.position);
    }
}

void PointCloud::render() {
    if (points.empty()) {
        spdlog::error("point cloud has no points to render");
        return;
    }

    f->glBindVertexArray(VAO);
    f->glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(points.size()));
    f->glBindVertexArray(0);
}
