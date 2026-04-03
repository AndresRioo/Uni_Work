#include "Materials/Transp.hpp"

Transp::Transp(const vec3& color): Material(color) {
    Kd = color;
}

Transp::Transp(const vec3& a, const vec3& d, const vec3& s, const float k):
    Material(a, d, s, k) {
}

Transp::Transp(const vec3& a, const vec3& d, const vec3& s, const float k, const float o):
    Material(a, d, s, k, o) {
}

Transp::~Transp() {
}
bool Transp::evaluate(const Ray& r_in, float t, vec3 normal, vec3& color, Ray & r_out) const  {
    // Obtener el origen del rayo y la dirección reflejada
    glm::vec3 origin = r_in.origin + t * r_in.direction;

    float nu_i = 1.0f;  // Índice de refracción del medio inicial aire
    float nu_t = this->ior; // Índice de refracción del material Transp

    // Comprobar si el rayo entra o sale del material
    float cos_theta = glm::dot(-r_in.direction, normal);
    if (cos_theta < 0) {
        std::swap(nu_i, nu_t);
        normal = -normal;  // Invertir la normal si el rayo está saliendo
        cos_theta = glm::dot(-r_in.direction, normal);

    }

    glm::vec3 refractedDirection = glm::refract(r_in.direction, normal, nu_i / nu_t); // Calcula la dirección refractada

    if (glm::length(refractedDirection) > 0) {  // Si se transmite (transparente)
        color = Kt; // Aplica la opacidad al color
        origin += normal  * 0.001f;  // Pequeño desplazamiento para evitar autointersección
        r_out = Ray(origin, refractedDirection);
        return true;


    } else {  // Reflexión interna total
        color = Ks; // Aplica la opacidad al color reflejado
        glm::vec3 reflectedDir = glm::reflect(r_in.direction, normal);
        origin += reflectedDir * 0.001f;
        r_out = Ray(origin, reflectedDir);
        return true;
    }
}

bool Transp::evaluate(const Ray& r_in, float t, vec3 normal, vec3& color, std::vector<Ray>& rays_out, int k) const {

    // todo
    return false;
}



vec3 Transp::getDiffuse(vec2 uv) const {
    return Kd;
}
