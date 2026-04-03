#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <random>

#include "Utilities/Ray.hpp"

using namespace glm;

// Classe abstracte Material. 
// Totes les seves filles hauran de definir el metode abstracte "evaluate" implementat
class Material {
public:
    /*
    Material(): Ka(1.0f), Kd(1.0f), Ks(1.0f) {
        shininess = 1.0f;
    };
    */

    // Constructor por defecto que inicializa los valores predeterminados de la fitxa 2
    Material() : Ka(0.2f, 0.2f, 0.2f), Kd(1.0f), Ks(0.8f, 0.8f, 0.8f), shininess(100.0f) {
        albedo = Kd; // El albedo se inicializa con el color difuso (Kd)
    }

    Material(vec3 d):  Ka(1.0f), Kd(d), Ks(1.0f) {
        shininess = 100.0f;
        albedo = d;
    };

    Material(vec3 a, vec3 d, vec3 s, float shininess):  Ka(a), Kd(d), Ks(s), albedo(d), shininess(shininess) {};

    Material(vec3 a, vec3 d, vec3 s, float shininess, float opacity):  Ka(a), Kd(d), Ks(s), albedo(d), shininess(shininess), opacity(opacity) {};
    ~Material() {};

    virtual bool evaluate(const Ray& r_in, float t, vec3 normal, vec3& color, Ray & r_out) const = 0;

    virtual bool evaluate(const Ray& r_in, float t, vec3 normal, vec3& color, std::vector<Ray>& rays_out, int k) const = 0;


    virtual vec3 getDiffuse(vec2 point) const { return Kd; };
    
    virtual vec3 getColor(vec2 point) const { return albedo; };
    virtual float getRoughness(vec2 point) const { return roughness; };

    

    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    vec3 Kt;

    vec3 albedo;
    float roughness;

    float shininess;
    float opacity; // opacity es la fraccio de 0..1 (0 és totalment transparent, 1 és totalment opac)

    float ior; // Índice de refracción del material (nu_t)
    

};
