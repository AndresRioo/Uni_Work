#version 330 core

in vec3 fragNormal;
in vec3 fragPosition;

out vec4 fragColor;

uniform vec3 viewPos;

struct LlumPuntual {
    vec3 pos;
    float a;
    float b;
    float c;
};

struct LlumComponents {
    vec3 ambient;
    vec3 difosa;
    vec3 especular;
    int enabled;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform int numLights;
uniform LlumPuntual llumsPuntuals[8];
uniform LlumComponents llumsComponents[8];
uniform Material material;
uniform vec3 ambientGlobal;

void main()
{
    vec3 N = normalize(fragNormal);
    vec3 V = normalize(viewPos - fragPosition);
    vec3 result = vec3(0.0);

    for (int i = 0; i < numLights; ++i) {
        if (llumsComponents[i].enabled == 1) {
            vec3 L = normalize(llumsPuntuals[i].pos - fragPosition);
            float d = length(llumsPuntuals[i].pos - fragPosition);
            float att = 1.0 / (llumsPuntuals[i].a + llumsPuntuals[i].b * d + llumsPuntuals[i].c * d * d);

            float NdotL = max(dot(N, L), 0.0);

            // === Toon shading ===
            float intensity;
            if (NdotL > 0.95)
                intensity = 1.0;
            else if (NdotL > 0.5)
                intensity = 0.7;
            else if (NdotL > 0.25)
                intensity = 0.4;
            else
                intensity = 0.1;

            vec3 diffuse = llumsComponents[i].difosa * material.diffuse * intensity;
            result += att * diffuse;
        }
    }

    // Silueta 
    float cosAlpha = dot(N, V);
    float outlineFactor = 1.0 - abs(cosAlpha);  // major com més de perfil mires
    vec3 edgeColor = material.diffuse * outlineFactor;

    // Suma toon shading + silueta 
    result += edgeColor;

    result += ambientGlobal * material.ambient;

    fragColor = vec4(clamp(result, 0.0, 1.0), 1.0);
}
