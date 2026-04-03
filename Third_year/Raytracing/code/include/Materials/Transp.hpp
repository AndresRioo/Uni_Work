#pragma once

#include "Materials/Material.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp> // Per a glm::ballRand

class Transp : public Material { 
    public:

    

    Transp() {};
    Transp(const vec3& color);
    Transp(const vec3& a, const vec3& d, const vec3& s, const float k);
    Transp(const vec3& a, const vec3& d, const vec3& s, const float k, const float o);
    virtual ~Transp();

    virtual bool evaluate(const Ray& r_in, float t, vec3 normal, vec3& color, Ray & r_out) const override;
    virtual bool evaluate(const Ray& r_in, float t, vec3 normal, vec3& color, std::vector<Ray>& rays_out, int k) const override;

    virtual vec3 getDiffuse(vec2 point) const override;

    virtual float getDmax() const { return dmax; };
    virtual void setDmax(float d) { dmax = d; };

    private:
    float dmax = 100.0f;
};
