#ifndef SHADER_H
#define SHADER_H

#include <QOpenGLFunctions_4_5_Core>
#include <glm/glm.hpp>
#include <string>

class Shader {
public:
    Shader() = default;
    Shader(
        QOpenGLFunctions_4_5_Core* f,
        const std::string& vertex_path,
        const std::string& fragment_path
    );
    ~Shader();

    void bind();
    void unbind();
    void setMat4(const std::string &name, const glm::mat4& v) const;
    void setMat3(const std::string &name, const glm::mat3& v) const;
    void setMat2(const std::string &name, const glm::mat2& v) const;
    void setVec4(const std::string &name, const glm::vec4& v) const;
    void setVec3(const std::string &name, const glm::vec3& v) const;
    void setVec2(const std::string &name, const glm::vec2& v) const;
    void setInt(const std::string &name, int v) const;
    void setFloat(const std::string &name, float v) const;

private:
    QOpenGLFunctions_4_5_Core* f;
    GLuint program = 0;

    GLuint compile(GLenum type, const std::string& source) const;

    static std::string readFile(const std::string& path);
};

#endif // SHADER_H