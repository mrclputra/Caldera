#include "Shader.h"
#include <spdlog/spdlog.h>

#include <fstream>
#include <sstream>
#include <glm/glm.hpp>

#include "glm/gtc/type_ptr.inl"

Shader::Shader(
    QOpenGLFunctions_4_5_Core* f,
    const std::string& vertexPath,
    const std::string& fragmentPath
) {
    this->f = f;

    GLuint vert = compile(GL_VERTEX_SHADER, readFile(vertexPath));
    GLuint frag = compile(GL_FRAGMENT_SHADER, readFile(fragmentPath));

    program = f->glCreateProgram();
    f->glAttachShader(program, vert);
    f->glAttachShader(program, frag);
    f->glLinkProgram(program);

    GLint ok = GL_FALSE;
    f->glGetProgramiv(program, GL_LINK_STATUS, &ok);
    if (!ok) {
        GLchar infoLog[1024];
        f->glGetProgramInfoLog(program, 1024, nullptr, infoLog);
        spdlog::error("PROGRAM_LINKING_ERROR: {}", infoLog);
        f->glDeleteProgram(program);
        f->glDeleteShader(vert);
        f->glDeleteShader(frag);
        throw std::runtime_error("PROGRAM_LINKING_ERROR"); // todo: find better handling method?
    }

    f->glDeleteShader(vert);
    f->glDeleteShader(frag);
}

Shader::~Shader() {
    f->glDeleteProgram(program);
}


void Shader::bind() {
    f->glUseProgram(program);
}
void Shader::unbind() {
    f->glUseProgram(0);
}


void Shader::setMat4(const std::string &name, const glm::mat4 &v) const {
    f->glUniformMatrix4fv(f->glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, glm::value_ptr(v));
}
void Shader::setMat3(const std::string &name, const glm::mat3 &v) const {
    f->glUniformMatrix3fv(f->glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, glm::value_ptr(v));
}
void Shader::setMat2(const std::string &name, const glm::mat2 &v) const {
    f->glUniformMatrix2fv(f->glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, glm::value_ptr(v));
}
void Shader::setVec4(const std::string &name, const glm::vec4 &v) const {
    f->glUniform4fv(f->glGetUniformLocation(program, name.c_str()), 1, glm::value_ptr(v));
}
void Shader::setVec3(const std::string &name, const glm::vec3 &v) const {
    f->glUniform3fv(f->glGetUniformLocation(program, name.c_str()), 1, glm::value_ptr(v));
}
void Shader::setVec2(const std::string &name, const glm::vec2 &v) const {
    f->glUniform2fv(f->glGetUniformLocation(program, name.c_str()), 1, glm::value_ptr(v));
}
void Shader::setInt(const std::string &name, int v) const {
    // *use this for booleans
    f->glUniform1i(f->glGetUniformLocation(program, name.c_str()), v);
}
void Shader::setFloat(const std::string &name, float v) const {
    f->glUniform1f(f->glGetUniformLocation(program, name.c_str()), v);
}


GLuint Shader::compile(GLenum type, const std::string& source) const {
    GLuint shader = f->glCreateShader(type);
    const char* c = source.c_str();
    f->glShaderSource(shader, 1, &c, nullptr);
    f->glCompileShader(shader);
    GLint ok = GL_FALSE;
    f->glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        GLchar infoLog[1024];
        f->glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        spdlog::error("SHADER_COMPILE_ERROR: {}", infoLog);
        f->glDeleteShader(shader);
        return 0;
    }
    return shader;
}

// TODO: move to a global utils file
std::string Shader::readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file) throw std::runtime_error("Could not open file: " + path);
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}


