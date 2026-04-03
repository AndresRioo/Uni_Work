#include "Materials/Metal.hpp"

Metal::Metal(const vec3& color): Material(color) {
    Kd = color;
}

Metal::Metal(const vec3& a, const vec3& d, const vec3& s, const float k):
    Material(a, d, s, k) {
}

Metal::Metal(const vec3& a, const vec3& d, const vec3& s, const float k, const float o):
    Material(a, d, s, k, o) {
}

Metal::~Metal() {
}

bool Metal::evaluate(const Ray& r_in, float t, vec3 normal, vec3& color, Ray & r_out) const  {
    
    color = Ks;

    // Obtener el origen del rayo y la dirección reflejada

    // punto interseccion = punto origen + distancia * direccion rayo incidencia 

    glm::vec3 origin = r_in.origin + t * r_in.direction;
    glm::vec3 reflectedDirection = glm::reflect(r_in.direction, normal); // Calcula la dirección reflejada

    // Crea el nuevo rayo reflejado con el mismo origen y la dirección reflejada
    r_out = Ray(origin, reflectedDirection);

    return true ;
}

bool Metal::evaluate(const Ray& r_in, float t, vec3 normal, vec3& color, std::vector<Ray>& rays_out, int k) const {

    // todo
    return false;
}


vec3 Metal::getDiffuse(vec2 uv) const {
    return Kd;
}
