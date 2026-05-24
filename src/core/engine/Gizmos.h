#ifndef GIZMOS_H
#define GIZMOS_H

#include <QOpenGLFunctions_4_5_Core>
#include <glm/glm.hpp>
#include <memory>

#include "components/Shader.h"

struct BoundingBox {
    glm::vec3 min{0.0f};
    glm::vec3 max{0.0f};
};

class Gizmos {
public:
    explicit Gizmos(QOpenGLFunctions_4_5_Core* f);
    ~Gizmos();

    void initialize(Shader* s);
    void drawBoundingBox(
        const BoundingBox& bb,
        const glm::mat4& model,
        const glm::mat4& view,
        const glm::mat4& proj,
        glm::vec4 color = {1.0f, 1.0f, 0.0f, 1.0f}
    );

private:
    QOpenGLFunctions_4_5_Core* f;
    Shader* shader = nullptr;
    GLuint VAO = 0;
    GLuint VBO = 0;
};

#endif // GIZMOS_H