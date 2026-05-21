#version 460 core
out vec4 FragColor;

uniform vec3 viewPos;

in vec3 vFragPos;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBitangent;
in vec2 vTexCoords;

void main() {
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(vec3(1.0, 2.0, 1.0)); // this is placeholder, to be replaced with actual directionals later on

    vec4 uAlbedo = vec4(255, 0, 255, 255); // this is placeholder, for meshes with no texture maps

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 ambient = 0.15 * uAlbedo.rgb;
    vec3 diffuse = diff * uAlbedo.rgb;

    FragColor = vec4(ambient + diffuse, uAlbedo.a);
}