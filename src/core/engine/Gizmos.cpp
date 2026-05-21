#include "Gizmos.h"

Gizmos::Gizmos(QOpenGLFunctions_4_5_Core* f) : f(f) {}

Gizmos::~Gizmos() {
    if (VBO) f->glDeleteBuffers(1, &VBO);
    if (VAO) f->glDeleteVertexArrays(1, &VAO);
}

void Gizmos::initialize() {
    shader = std::make_shared<Shader>(
        f,
        std::string(SHADER_DIR) + "gizmo.vert",
        std::string(SHADER_DIR) + "gizmo.frag"
    );

    f->glGenVertexArrays(1, &VAO);
    f->glGenBuffers(1, &VBO);

    f->glBindVertexArray(VAO);
    f->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    f->glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW);
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
    f->glBindVertexArray(0);
}

void Gizmos::drawBoundingBox(
    const BoundingBox& bb,
    const glm::mat4& model,
    const glm::mat4& view,
    const glm::mat4& proj,
    glm::vec4 color
) {
    const glm::vec3& mn = bb.min;
    const glm::vec3& mx = bb.max;

    glm::vec3 verts[24] = {
        // bottom face
        {mn.x,mn.y,mn.z}, {mx.x,mn.y,mn.z},
        {mx.x,mn.y,mn.z}, {mx.x,mx.y,mn.z},
        {mx.x,mx.y,mn.z}, {mn.x,mx.y,mn.z},
        {mn.x,mx.y,mn.z}, {mn.x,mn.y,mn.z},
        // top face
        {mn.x,mn.y,mx.z}, {mx.x,mn.y,mx.z},
        {mx.x,mn.y,mx.z}, {mx.x,mx.y,mx.z},
        {mx.x,mx.y,mx.z}, {mn.x,mx.y,mx.z},
        {mn.x,mx.y,mx.z}, {mn.x,mn.y,mx.z},
        // verticals
        {mn.x,mn.y,mn.z}, {mn.x,mn.y,mx.z},
        {mx.x,mn.y,mn.z}, {mx.x,mn.y,mx.z},
        {mx.x,mx.y,mn.z}, {mx.x,mx.y,mx.z},
        {mn.x,mx.y,mn.z}, {mn.x,mx.y,mx.z},
    };

    f->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    f->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);

    shader->bind();
    shader->setMat4("model", model);
    shader->setMat4("view", view);
    shader->setMat4("proj", proj);
    shader->setVec4("color", color);

    f->glBindVertexArray(VAO);
    f->glDrawArrays(GL_LINES, 0, 24);
    f->glBindVertexArray(0);
}
