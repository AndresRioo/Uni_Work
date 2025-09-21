#version 330 core

in vec3 materialColor;
out vec4 colorOut;

void main()
{
    colorOut = vec4(materialColor, 1.0);
}
