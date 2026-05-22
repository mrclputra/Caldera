#version 450 core

in vec3 vNormal;
in vec4 vColor;

out vec4 FragColor;

const vec3 light_dir = normalize(vec3(0.5, 1.0, 0.75));
const vec3 light_color = vec3(1.0);
const float ambient = 0.45;

void main() {
    vec2 c = gl_PointCoord * 2.0 - 1.0;
    if (dot(c, c) > 1.0) discard;

    vec3 n = normalize(vNormal);
    float diff = max(dot(n, light_dir), 0.0);

    vec3 lit = (ambient + diff * (1.0 - ambient)) * light_color * vColor.rgb;
    FragColor = vec4(lit, vColor.a);
}
