#pragma once

#include "Renders/Render.hpp"
#include "GeometricObjects/Scene.hpp"
#include "Lights/Light.hpp"
#include "Camera.hpp"
#include "Config.hpp"


class Raytracing : public Render {
    shared_ptr<Scene> scene;
    vector<shared_ptr<Light>> lights;
    shared_ptr<Camera> camera;
    shared_ptr<Config> config;


    shared_ptr<ShadingStrategy> shadingStrategy;

public:
    Raytracing(shared_ptr<Scene> scene, vector<shared_ptr<Light>> lights, shared_ptr<Camera> camera, shared_ptr<Config> config , shared_ptr<ShadingStrategy> shadingStrategy) {
        this->scene = scene;
        this->lights = lights;
        this->camera = camera;
        this->config = config;
        this->shadingStrategy = shadingStrategy;
    };

    void setShadingStrategy(shared_ptr<ShadingStrategy> strategy);

    virtual ~Raytracing() {};

    virtual glm::vec3 tracer(Ray ray) const override;
    glm::vec3 tracerRecursive(Ray ray, int depth) const;
    glm::vec3 tracerRecursiveNoBackground(Ray ray, int depth) const;
    // metodo para que el reflejo no tenga en cuenta el fondo (apartado 4 fitxa 2)
};
