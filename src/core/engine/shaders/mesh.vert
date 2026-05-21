#version 460 core
layout (location = 0) in vec3 aPos;         // vertex position
layout (location = 1) in vec3 aNormal;      // vertex normal
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aBitangent;
layout (location = 4) in vec2 aTexCoords;   // vertex texture coordinates

out vec3 vFragPos;
out vec3 vNormal;
out vec3 vTangent;
out vec3 vBitangent;
out vec2 vTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    mat3 M = mat3(model);

    vFragPos = vec3(model * vec4(aPos, 1.0));
    vNormal = M * aNormal;
    vTangent = M * aTangent;
    vBitangent = M * aBitangent;
    vTexCoords = aTexCoords;

    gl_Position = proj * view * vec4(vFragPos, 1.0); // TO NDC
}