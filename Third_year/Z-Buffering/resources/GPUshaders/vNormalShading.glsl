#version 330 core

layout(location = 0) in vec4 vPosition;   // Posición del vértice
layout(location = 3) in vec4 vNormal;     // Normal del vértice

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 normalColor;

void main()
{
    // Transformación de la posición
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vPosition;

    // Transformación de la normal al espacio del mundo
    vec3 normal = normalize(mat3(modelMatrix) * vNormal.xyz);

    // Mapeamos [-1,1] a [0,1] para visualizar como color
    normalColor = normal * 0.5 + 0.5;
}
