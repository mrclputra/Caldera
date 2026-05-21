#version 460 core

in vec3 vNormal;
in vec4 vColor;

out vec4 FragColor;

void main() {
    // this will discard fragment corners
    vec2 c = gl_PointCoord * 2.0 - 1.0;
    if (dot(c, c) > 1.0) discard;

    FragColor = vColor;
}
