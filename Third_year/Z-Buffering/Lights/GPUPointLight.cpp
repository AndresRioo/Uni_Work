#include "GPUPointLight.hpp"

GPUPointLight::GPUPointLight(vec3 posicio, vec3 Ia, vec3 Id, vec3 Is): GPULight(Ia, Id, Is) {
    this->pos = posicio;
    this->a = 0.0f;
    this->b = 0.0f;
    this->c = 0.0f;
}

GPUPointLight::GPUPointLight(vec3 posicio, vec3 Ia, vec3 Id, vec3 Is, float a, float b, float c): GPULight(Ia, Id, Is) {
    this->pos = posicio;
    this->a = a;
    this->b = b;
    this->c = c;
}

GPUPointLight::GPUPointLight(vec3 posicio, vec3 Ia, vec3 Id, vec3 Is, float a, float b, float c, bool isEnabled): GPULight(Ia, Id, Is, isEnabled) {
    this->pos = posicio;
    this->a = a;
    this->b = b;
    this->c = c;
}

vec3 GPUPointLight::vectorL(vec3 point) {
    return normalize(pos - point);
}

float GPUPointLight::attenuation(vec3 point) {
    if (abs(a)<DBL_EPSILON && abs(b)<DBL_EPSILON  && abs(c)<DBL_EPSILON) {
        //Si tots els coeficients son 0 considerem que no hi ha atenuacio
        return 1.0f;
    }
    //Calculem la distancia entre el punt i la posicio de la llum
    float d = distance(point, pos);
    return 1.0f/(c*d*d + b*d + a);
}

float GPUPointLight::distanceToLight(vec3 point) {
    return distance(point, pos);
}

void GPUPointLight:: toGPU(GLuint pr) {
    GPULight::toGPU(pr);
    // TO DO: PAS 2.2. enviar la posició de la llum puntual a la GPU  i els coeficients d'atenuació
    // Cal obtenir l'identificador de la variable uniform de la GPU
    // i actualitzar el seu valor amb la posició de la llum puntual

    program = pr;

    /*
    struct {
        GLuint pos;
        GLuint a;
        GLuint b;
        GLuint c;
    } llumPuntualComponents;

    llumPuntualComponents.pos = glGetUniformLocation(program, "llumPuntual.pos");
    llumPuntualComponents.a   = glGetUniformLocation(program, "llumPuntual.a");
    llumPuntualComponents.b   = glGetUniformLocation(program, "llumPuntual.b");
    llumPuntualComponents.c   = glGetUniformLocation(program, "llumPuntual.c");

    glUniform3fv(llumPuntualComponents.pos, 1, glm::value_ptr(this->pos));
    glUniform1f(llumPuntualComponents.a, this->a);
    glUniform1f(llumPuntualComponents.b, this->b);
    glUniform1f(llumPuntualComponents.c, this->c);
    */

}

void GPUPointLight::updateToGPU(int index) {
    // TO DO: PAS 2.3: actualitzar les propietats de la llum puntual a la GPU
    // Cal obtenir els identificadors de les variables uniform de la GPU de la llum amb index "index"
    // i actualitzar els seus valors amb les propietats de la llum puntual
    GPULight::updateToGPU(index);

    std::string idx = std::to_string(index);

    GLint locPos = glGetUniformLocation(program, ("llumsPuntuals[" + idx + "].pos").c_str());
    GLint locA   = glGetUniformLocation(program, ("llumsPuntuals[" + idx + "].a").c_str());
    GLint locB   = glGetUniformLocation(program, ("llumsPuntuals[" + idx + "].b").c_str());
    GLint locC   = glGetUniformLocation(program, ("llumsPuntuals[" + idx + "].c").c_str());

    glUniform3fv(locPos, 1, glm::value_ptr(pos));
    glUniform1f(locA, a);
    glUniform1f(locB, b);
    glUniform1f(locC, c);

}
