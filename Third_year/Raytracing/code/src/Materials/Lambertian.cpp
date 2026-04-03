#include "Materials/Lambertian.hpp"

Lambertian::Lambertian(const vec3& color): Material(color) {
    Kd = color;
}

Lambertian::Lambertian(const vec3& a, const vec3& d, const vec3& s, const float k):
    Material(a, d, s, k) {
}

Lambertian::Lambertian(const vec3& a, const vec3& d, const vec3& s, const float k, const float o):
    Material(a, d, s, k, o) {
}

Lambertian::~Lambertian() {
}

bool Lambertian::evaluate(const Ray& r_in, float t, vec3 normal, vec3& color, Ray & r_out) const  {
    vec3 rec = r_in(t);
    vec3 target = rec + normal + glm::ballRand(1.0f);
    r_out =  Ray(rec, target-rec);
    color = Kd;
    return false;
}

bool Lambertian::evaluate(const Ray& r_in, float t, vec3 normal, vec3& color, std::vector<Ray>& rays_out, int k) const {
    
    int Nraigs = 1 + k*roughness; // nombre de raigs secundaris
    
    vec3 rec = r_in(t); // punt d'intersecció 

    for (int i = 0; i < Nraigs; ++i) { // generar Nraigs

        vec3 target = rec + normal + glm::ballRand(1.0f); 
        rays_out.emplace_back(rec, target - rec);  // Es genera un punt target aleatori a l'hemisferi definit per normal.

    }
    color = Kd;
    return true; // indicar que s'han generat raigs secundaris
}

vec3 Lambertian::getDiffuse(vec2 uv) const {
    return Kd;
}
