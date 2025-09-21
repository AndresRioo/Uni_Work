#include "Materials/MaterialTextura.hpp"


MaterialTextura::MaterialTextura(const vec3& color): Material(color) {
    Kd = color;
}

MaterialTextura::MaterialTextura(const vec3& a, const vec3& d, const vec3& s, const float k):
    Material(a, d, s, k) {
}

MaterialTextura::MaterialTextura(const vec3& a, const vec3& d, const vec3& s, const float k, const float o):
    Material(a, d, s, k, o) {
}

MaterialTextura::~MaterialTextura() {
}

bool MaterialTextura::evaluate(const Ray& r_in, float t, vec3 normal, vec3& color, Ray & r_out) const  {
    
    /*
    vec3 rec = r_in(t);
    vec3 target = rec + normal + glm::ballRand(1.0f);
    r_out =  Ray(rec, target-rec);

    //color = getDiffuse(uv);
    color = Kd; // ???

    */

    return false;


}

bool MaterialTextura::evaluate(const Ray& r_in, float t, vec3 normal, vec3& color, std::vector<Ray>& rays_out, int k) const {

    // todo
    return false;
}


vec3 MaterialTextura::getDiffuse(vec2 uv) const {
        
    if (!background) { // foto nula
        return glm::vec3(1.0f, 1.0f, 1.0f); // Devuelve blanco si no hay textura
    }

    return background->getPixelColor(uv);

}
