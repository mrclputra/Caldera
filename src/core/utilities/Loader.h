#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <memory>
#include <string>
#include <QOpenGLFunctions_4_5_Core>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <spdlog/spdlog.h>

#include "engine/components/PointCloud.h"

inline std::shared_ptr<PointCloud> loadPointCloud(QOpenGLFunctions_4_5_Core* f, const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_GenNormals);

    if (!scene || !scene->mRootNode) {
        spdlog::error("loader: {}", importer.GetErrorString());
        return nullptr;
    }

    std::vector<PointCloud::Point> points;

    for (unsigned int m = 0; m < scene->mNumMeshes; m++) {
        aiMesh* mesh = scene->mMeshes[m];
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            PointCloud::Point p{};
            p.position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };

            if (mesh->HasNormals())
                p.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

            if (mesh->HasVertexColors(0)) {
                auto& c = mesh->mColors[0][i];
                p.color = { c.r, c.g, c.b, c.a };
            } else {
                p.color = { 1.0f, 0.0f, 1.0f, 1.0f };
            }

            points.push_back(p);
        }
    }

    if (points.empty()) {
        spdlog::error("loader: no vertices found in {}", path);
        return nullptr;
    }

    glm::vec3 mn = points[0].position, mx = points[0].position;
    for (const auto& p : points) {
        mn = glm::min(mn, p.position);
        mx = glm::max(mx, p.position);
    }

    // center point cloud at 0,0
    const glm::vec3 center = (mn + mx) * 0.5f;
    for (auto& p : points) {
        p.position -= center;
        mn -= center;
        mx -= center;
    }

    spdlog::info("loaded {} points | bbox ({:.2f},{:.2f},{:.2f}) - ({:.2f},{:.2f},{:.2f})",
        points.size(), mn.x, mn.y, mn.z, mx.x, mx.y, mx.z);
    spdlog::info("loaded center ({:.2f},{:.2f},{:.2f}) | diagonal {:.2f}",
        ((mn + mx) * 0.5f).x, ((mn + mx) * 0.5f).y, ((mn + mx) * 0.5f).z, glm::length(mx - mn));

    return std::make_shared<PointCloud>(f, path, std::move(points));
}

#endif // MODELLOADER_H
