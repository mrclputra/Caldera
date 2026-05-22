#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;

out vec3 vNormal;
out vec4 vColor;

uniform mat4 model, view, proj;
uniform float pointSize;

void main() {
    vNormal = mat3(transpose(inverse(model))) * aNormal;
    vColor = aColor;
    gl_PointSize = pointSize;
    gl_Position = proj * view * model * vec4(aPos, 1.0);
}
