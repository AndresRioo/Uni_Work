#version 330 core

layout(location = 0) in vec4 vPosition;
layout(location = 3) in vec4 vNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 fragNormal;
out vec3 fragPosition;

void main()
{
    vec4 worldPos = modelMatrix * vPosition;
    fragPosition = vec3(worldPos);

    fragNormal = normalize(mat3(transpose(inverse(modelMatrix))) * vNormal.xyz);

    gl_Position = projectionMatrix * viewMatrix * worldPos;
}
