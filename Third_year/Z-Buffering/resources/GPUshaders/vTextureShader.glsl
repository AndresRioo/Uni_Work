#version 330 core

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec2 vTexCoord;
layout(location = 3) in vec4 vNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 fragNormal;
out vec3 fragPosition;
out vec2 fragTexCoord;

void main() {


    vec4 worldPos = modelMatrix * vPosition; // phong
    fragPosition = vec3(worldPos); // phong
    fragNormal = normalize(mat3(transpose(inverse(modelMatrix))) * vNormal.xyz); // phong

    fragTexCoord = vTexCoord; // texture

    gl_Position = projectionMatrix * viewMatrix * worldPos;
}
