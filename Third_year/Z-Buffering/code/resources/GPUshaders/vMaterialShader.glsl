#version 330 core

layout (location = 0) in vec4 vPosition;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 materialColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vPosition;

    // materialColor = material.ambient;   // para testear Ka
    //materialColor = material.diffuse;      // para testear Kd
    materialColor = material.specular;  // para testear Ks
}
