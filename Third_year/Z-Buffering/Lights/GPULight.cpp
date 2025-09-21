#include "Lights/GPULight.hpp"

void GPULight::toGPU(GLuint p) {
    // TO DO: enviar les propietats de Ia, Id, Is i enabled a la GPU. Pas 2.2
    program = p;

    /*

    1 sola llum

    struct {
        GLint ambient;
        GLint difosa;
        GLint especular;
        GLint enabled;
    } componentsLlumLoc;

    componentsLlumLoc.ambient   = glGetUniformLocation(program, "llumComponents.ambient");
    componentsLlumLoc.difosa    = glGetUniformLocation(program, "llumComponents.difosa");
    componentsLlumLoc.especular = glGetUniformLocation(program, "llumComponents.especular");
    componentsLlumLoc.enabled   = glGetUniformLocation(program, "llumComponents.enabled");

    glUniform3fv(componentsLlumLoc.ambient,   1, glm::value_ptr(this->getIa()));
    glUniform3fv(componentsLlumLoc.difosa,    1, glm::value_ptr(this->getId()));
    glUniform3fv(componentsLlumLoc.especular, 1, glm::value_ptr(this->getIs()));
    glUniform1i(componentsLlumLoc.enabled, this->enabled ? 1 : 0);
    */
}


void GPULight::updateToGPU(int index) {
    // TO DO: actualitzar les propietats de la llum a la GPU. Pas 2.3
    // Cal obtenir els identificadors de les variables uniform de la GPU
    // i actualitzar els seus valors amb les propietats de la llum
    
    std::string idx = std::to_string(index);

    GLint locAmbient   = glGetUniformLocation(program, ("llumsComponents[" + idx + "].ambient").c_str());
    GLint locDifosa    = glGetUniformLocation(program, ("llumsComponents[" + idx + "].difosa").c_str());
    GLint locEspecular = glGetUniformLocation(program, ("llumsComponents[" + idx + "].especular").c_str());
    GLint locEnabled   = glGetUniformLocation(program, ("llumsComponents[" + idx + "].enabled").c_str());

    glUniform3fv(locAmbient,   1, glm::value_ptr(this->getIa()));
    glUniform3fv(locDifosa,    1, glm::value_ptr(this->getId()));
    glUniform3fv(locEspecular, 1, glm::value_ptr(this->getIs()));
    glUniform1i(locEnabled, this->enabled ? 1 : 0);

    
}
