#pragma once

#include <QOpenGLFunctions_4_5_Core>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Mesh {
public:
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 tangent;
        glm::vec3 bitangent;
        glm::vec2 uv;
    };

    Mesh(
        QOpenGLFunctions_4_5_Core* f,
        std::vector<Vertex> vertices,
        std::vector<GLuint> indices
    );
    ~Mesh();

    void upload();
    void render();

private:
    QOpenGLFunctions_4_5_Core* f;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;
};

