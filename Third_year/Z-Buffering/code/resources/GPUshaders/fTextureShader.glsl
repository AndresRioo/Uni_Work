#version 330 core

in vec3 fragNormal;
in vec3 fragPosition;
in vec2 fragTexCoord;

out vec4 fragColor;

uniform vec3 viewPos;
uniform vec3 ambientGlobal;

uniform sampler2D textureMap;

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
    vec3 difosa;
    vec3 specular;
    float shininess;
};

uniform int numLights;
uniform LlumPuntual llumsPuntuals[8];
uniform LlumComponents llumsComponents[8];
uniform Material material;

void main() {
    vec3 N = normalize(fragNormal);
    vec3 V = normalize(viewPos - fragPosition);
    vec3 result = vec3(0.0);

    vec3 texColor = texture(textureMap, fragTexCoord).rgb;

    for (int i = 0; i < 8; ++i) {
        if (llumsComponents[i].enabled == 1) {
            vec3 L = normalize(llumsPuntuals[i].pos - fragPosition);
            vec3 H = normalize(L + V);
            float d = length(llumsPuntuals[i].pos - fragPosition);
            float att = 1.0 / (llumsPuntuals[i].a + llumsPuntuals[i].b * d + llumsPuntuals[i].c * d * d);

            float NdotL = max(dot(N, L), 0.0);
            float NdotH = max(dot(N, H), 0.0);

            vec3 ambient = llumsComponents[i].ambient * material.ambient;
            
            vec3 baseDiffuse = 0.75 * texColor + 0.25 * material.difosa;
            vec3 diffuse = llumsComponents[i].difosa * baseDiffuse * NdotL;

            vec3 specular = llumsComponents[i].especular * material.specular * pow(NdotH, material.shininess);
            result += ambient + att * (diffuse + specular);
        }
    }

    result += ambientGlobal * material.ambient;
    fragColor = vec4(clamp(result, 0.0, 1.0), 1.0);
}
