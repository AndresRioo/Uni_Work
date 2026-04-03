#pragma once

#include "Materials/Material.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp> // Per a glm::ballRand


#include "Utilities/Image.hpp"



class MaterialTextura : public Material { 
    public:

    shared_ptr<Image> background;

    MaterialTextura(std::shared_ptr<Image> bg) : background(bg) {}

    MaterialTextura() {};
    MaterialTextura(const vec3& color);
    MaterialTextura(const vec3& a, const vec3& d, const vec3& s, const float k);
    MaterialTextura(const vec3& a, const vec3& d, const vec3& s, const float k, const float o);
    virtual ~MaterialTextura();

    virtual bool evaluate(const Ray& r_in, float t, vec3 normal, vec3& color, Ray & r_out) const override;
    virtual bool evaluate(const Ray& r_in, float t, vec3 normal, vec3& color, std::vector<Ray>& rays_out, int k) const override;

    virtual vec3 getDiffuse(vec2 point) const override;

};
