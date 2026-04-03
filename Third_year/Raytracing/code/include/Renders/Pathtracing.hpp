#pragma once

#include "Renders/Render.hpp"
#include "GeometricObjects/Scene.hpp"
#include "Lights/Light.hpp"
#include "Camera.hpp"
#include "Config.hpp"

class Pathtracing : public Render {
    shared_ptr<Scene> scene;
    vector<shared_ptr<Light>> lights;
    shared_ptr<Camera> camera;
    shared_ptr<Config> config;


    shared_ptr<ShadingStrategy> shadingStrategy;

public:
    Pathtracing(shared_ptr<Scene> scene, vector<shared_ptr<Light>> lights, shared_ptr<Camera> camera, shared_ptr<Config> config , shared_ptr<ShadingStrategy> shadingStrategy) {
        this->scene = scene;
        this->lights = lights;
        this->camera = camera;
        this->config = config;
        this->shadingStrategy = shadingStrategy;
    };

    void setShadingStrategy(shared_ptr<ShadingStrategy> strategy);

    virtual ~Pathtracing() {};

    virtual glm::vec3 tracer(Ray ray) const override;
    
    glm::vec3 tracerRecursive(Ray ray, int depth) const;
};
