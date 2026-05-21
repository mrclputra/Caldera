#include "Mesh.h"

#include "spdlog/spdlog.h"

Mesh::Mesh(QOpenGLFunctions_4_5_Core* f, std::vector<Vertex> vertices, std::vector<GLuint> indices) {
    this->f = f;
    this->vertices = std::move(vertices); // std::move avoids creating copies from the loader
    this->indices = std::move(indices);
    upload();
}

Mesh::~Mesh() {
    if (EBO) f->glDeleteBuffers(1, &EBO);
    if (VBO) f->glDeleteVertexArrays(1, &VBO);
    if (VAO) f->glDeleteVertexArrays(1, &VAO);
}

// upload to GPU
void Mesh::upload() {
    if (VAO) {
        spdlog::error("VAO already exists; cannot overwrite existing mesh data");
        return;
    }

    f->glGenVertexArrays(1, &VAO);
    f->glGenBuffers(1, &VBO);
    f->glGenBuffers(1, &EBO);

    f->glBindVertexArray(VAO);

    f->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    f->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    // vertex attributes
    f->glEnableVertexAttribArray(0); // position
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, position)));

    f->glEnableVertexAttribArray(1); // normal
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, normal)));

    f->glEnableVertexAttribArray(2); // tangent
    f->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, tangent)));

    f->glEnableVertexAttribArray(3); // bitangent
    f->glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, bitangent)));

    f->glEnableVertexAttribArray(4); // uv
    f->glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, uv)));

    f->glBindVertexArray(0);
}

void Mesh::render() {
    if (vertices.empty()) {
        spdlog::error("MESH HAS NO VERTICES TO RENDER");
        return;
    }

    f->glBindVertexArray(VAO);
    f->glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
    f->glBindVertexArray(0);
}
