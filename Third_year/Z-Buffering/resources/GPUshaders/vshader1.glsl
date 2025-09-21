#version 330 core

#define NUM_MAX_LLUMS 10

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec4 color;

uniform vec3 ambientGlobal;

uniform int numLights;

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
    int enabled; // int en vez de bool
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};


uniform LlumPuntual llumsPuntuals[8];
uniform LlumComponents llumsComponents[8];
uniform Material material;



void main() {
    // Calculate world space position
    gl_Position = projectionMatrix*viewMatrix*modelMatrix * vPosition;

    // Calculate the color out multiplying each factor ( rgb * light and alpha channel )
    
    // 1 struct sin array
    //color = vColor;
    //color = vec4(llumComponents.ambient, 1);  // Usar la componente difusa directamente
    //color = vec4(0.0, 0.0, numLights*0.25f, 1.0); // Si no funciona, entonces sabemos que algo está mal

    // array en el struct
    //color = vec4(llumsComponents[0].especular, 1); // funciona con array 0

    //color = vec4(llumsComponents[1].especular, 1); // no funciona con array 1


    //color = vec4( llumsComponents[0].ambient*0.25f + llumsComponents[1].ambient * 0.25f + llumsComponents[2].ambient * 0.25f , 1);


    //vec3 ambientTotal = vec3(0.0);

    //for (int i = 0; i < numLights; ++i) {
    //    ambientTotal += llumsComponents[i].ambient;
    //}

    //vec4 color = vec4(ambientTotal, 1.0); no va

    //color = vec4(llumsComponents[0].especular, 1); // funciona con array 0

    //if (llumsComponents[0].enabled == 1) {
    //    color = vec4(llumsComponents[0].especular, 1); // funciona con array 0
    //}

    //vec3 ambientTotal = vec3(0.0);

    vec3 ambientTotal = ambientGlobal;

    for (int i = 0; i < 8; ++i) {
        if (llumsComponents[i].enabled == 1) {
            ambientTotal += llumsComponents[i].difosa;
        } 
    }

    ambientTotal = clamp(ambientTotal, 0.0, 1.0);
    color = vec4(ambientTotal, 1.0);






}