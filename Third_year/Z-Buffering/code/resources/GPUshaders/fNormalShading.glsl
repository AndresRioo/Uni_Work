#version 330 core

in vec3 normalColor;
out vec4 fragColor;

void main()
{
    fragColor = vec4(normalColor, 1.0);  // Pintamos según normal
}
