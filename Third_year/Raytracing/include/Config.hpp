#pragma once

#include <memory>

#include <glm/glm.hpp>
#include "Camera.hpp"
#include "Utilities/Image.hpp"

using namespace glm;
using namespace std;

class Config {
public:
    int viewportWidth;
    int viewportHeight;
    vec3 observador;
    vec3 vrp;
    float fov;
    
    vec3 lightPos;
    vec3 lightAmbient;
    vec3 lightDiffuse;
    vec3 lightSpecular;
    

    vec3 lightAmbientGlobal;

    int numRays;
    int numSpheres;

    int backgroundMode;   // 0 = color, 1 = textura
    vec3 backgroundColor;
    shared_ptr<Image> background;

    int selectedTracer; // 0 = Raycasting, 1 = Raytracing, 2 = Pathtracing
    int selectedShader; // 0 = NormalShading, 1 = DiffuseShading, 2 = BlinnPhongShading

    bool shadow; 
    bool boundingBox = true;
    bool reflejaElBackground;
    
    int maxdepth;
    int numSamples;

    int KpathTracing;

    Config(int ww, int wh) {
        viewportWidth = ww;
        viewportHeight = wh;
        
        lightPos = vec3(0.0f, 5.0f, 0.0f);
        lightAmbient = vec3(0.1f, 0.1f, 0.1f);
        lightDiffuse = vec3(0.8f, 0.8f, 0.8f);
        lightSpecular = vec3(1.0f, 1.0f, 1.0f);


        lightAmbientGlobal = vec3(0.1f, 0.1f, 0.1f);

        numRays = 10;
        numSpheres = 3;

        //observador = vec3(9.315f, 3.744f, 6.918f);
        observador = vec3(0.0f, 0.0f, 0.0f);
        vrp = vec3(0.0f, 0.0f, -1.0f);
        fov = 90.0f;

        backgroundMode = false;
        backgroundColor = vec3(0.9f, 0.9f, 0.95f);
        background = nullptr;

        selectedTracer = 1;
        selectedShader = 3;

        reflejaElBackground = true;
        shadow = false;
        boundingBox = true;

        maxdepth = 3; // default ?
        numSamples = 1;

        KpathTracing = 0;

        
    }

    ~Config() {
        if (background) background->freeImage();
    }

    Config& operator=(const Config& other) {
        if (this != &other) { // Evitar autoassignació
            viewportWidth = other.viewportWidth;
            viewportHeight = other.viewportHeight;
            observador = other.observador;
            vrp = other.vrp;
            lightPos = other.lightPos;
            lightAmbient = other.lightAmbient;
            lightDiffuse = other.lightDiffuse;
            lightSpecular = other.lightSpecular;

            lightAmbientGlobal = other.lightAmbientGlobal;

            numRays = other.numRays;
            numSpheres = other.numSpheres;
            backgroundMode = other.backgroundMode;
            backgroundColor = other.backgroundColor;
            background = other.background;
            selectedTracer = other.selectedTracer;
            selectedShader = other.selectedShader;
            shadow = other.shadow;
            reflejaElBackground = other.reflejaElBackground;
            maxdepth = other.maxdepth;
            boundingBox = other.boundingBox;
            numSamples = other.numSamples;
            KpathTracing = other.KpathTracing;
        }
        return *this; // Retornem *this per permetre assignacions encadenades
    }
};
