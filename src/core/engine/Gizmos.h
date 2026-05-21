#pragma once

#include <QOpenGLFunctions_4_5_Core>
#include <glm/glm.hpp>
#include <memory>

#include "components/Shader.h"
#include "components/BoundingBox.h"

class Gizmos {
public:
    explicit Gizmos(QOpenGLFunctions_4_5_Core* f);
    ~Gizmos();

    void initialize();
    void drawBoundingBox(
        const BoundingBox& bb,
        const glm::mat4& model,
        const glm::mat4& view,
        const glm::mat4& proj,
        glm::vec4 color = {1.0f, 1.0f, 0.0f, 1.0f}
    );

private:
    QOpenGLFunctions_4_5_Core* f;
    std::shared_ptr<Shader> shader;
    GLuint VAO = 0;
    GLuint VBO = 0;
};
