#include "Loader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <spdlog/spdlog.h>
#include <glm/gtc/type_ptr.hpp>

std::vector<PointCloud::Point> loadPoints(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_GenNormals);

    if (!scene || !scene->mRootNode) {
        SPDLOG_ERROR("{}", importer.GetErrorString());
        return {};
    }

    // convert assimp root transform to glm and apply to all vertices
    const aiMatrix4x4& ai_root = scene->mRootNode->mTransformation;
    glm::mat4 root_transform = glm::transpose(glm::make_mat4(&ai_root.a1));
    glm::mat3 root_normal = glm::mat3(glm::transpose(glm::inverse(root_transform)));

    std::vector<PointCloud::Point> points;

    for (unsigned int m = 0; m < scene->mNumMeshes; m++) {
        aiMesh* mesh = scene->mMeshes[m];
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            PointCloud::Point p{};
            glm::vec4 pos = root_transform * glm::vec4(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1.0f);
            // glm::vec4 pos = glm::vec4(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1.0f);
            p.position = glm::vec3(pos);

            if (mesh->HasNormals()) {
                glm::vec3 n = root_normal * glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
                // glm::vec3 n = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
                p.normal = glm::normalize(n);
            }

            if (mesh->HasVertexColors(0)) {
                auto& c = mesh->mColors[0][i];
                p.color = { c.r, c.g, c.b, c.a };
            } else {
                p.color = { 1.0f, 0.0f, 1.0f, 1.0f };
            }

            points.push_back(p);
        }

        SPDLOG_INFO("processed model index {}", m);
    }

    if (points.empty()) {
        SPDLOG_ERROR("no vertices found in {}", path);
        return {};
    }

    glm::vec3 mn = points[0].position, mx = points[0].position;
    for (const auto& p : points) {
        mn = glm::min(mn, p.position);
        mx = glm::max(mx, p.position);
    }

    const glm::vec3 center = (mn + mx) * 0.5f;
    for (auto& p : points) {
        p.position -= center;
    }
    mn -= center;
    mx -= center;

    SPDLOG_INFO("found {} points | bbox ({:.2f},{:.2f},{:.2f}) - ({:.2f},{:.2f},{:.2f})",
        points.size(), mn.x, mn.y, mn.z, mx.x, mx.y, mx.z);
    SPDLOG_INFO("found center ({:.2f},{:.2f},{:.2f}) | diagonal {:.2f}",
        ((mn + mx) * 0.5f).x, ((mn + mx) * 0.5f).y, ((mn + mx) * 0.5f).z, glm::length(mx - mn));

    SPDLOG_INFO("completed point cloud load");
    return points;
}

std::shared_ptr<PointCloud> loadPointCloud(QOpenGLFunctions_4_5_Core* f, const std::string& path) {
    auto points = loadPoints(path);
    if (points.empty()) return nullptr;
    return std::make_shared<PointCloud>(f, path, std::move(points));
}

