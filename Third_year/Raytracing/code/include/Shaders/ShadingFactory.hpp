#pragma once

#include "Shaders/NormalShading.hpp"
#include "Shaders/DiffuseShading.hpp"
#include "Shaders/BlinnPhongShading.hpp"
#include "Shaders/ToonShading.hpp"


class ShadingFactory {
public:
    ShadingFactory() {};
    
    typedef enum  SHADING_TYPES{
        NORMALSHADING,
        DIFFUSESHADING,
        BLINNPHONGSHADING
    } SHADING_TYPES;

    static ShadingFactory& getInstance() {
        static ShadingFactory instance;
        return instance;
    }

    shared_ptr<ShadingStrategy>   createShading(SHADING_TYPES t, shared_ptr<Scene> scene, vector<shared_ptr<Light>> lights, vec3 lookFrom, bool shadow);
    ShadingFactory::SHADING_TYPES getIndexType(shared_ptr<ShadingStrategy> m);
};
