#include "GPUMaterial.hpp"


GPUMaterial::GPUMaterial(): Ka(0.2f), Kd(1.0f), Ks(0.8f) {
    shininess = 100.0f;
    opacity = 1;
};
GPUMaterial::GPUMaterial(vec3 d):  Ka(0.2f), Kd(d), Ks(0.8f) {
    shininess = 100.0f;
    opacity = 1;
};

GPUMaterial::GPUMaterial(vec3 a, vec3 d, vec3 s, float shininess):  Ka(a), Kd(d), Ks(s), shininess(shininess) {
    opacity = 1;
};

void GPUMaterial::toGPU(GLuint program) {
    // Set material properties to GPU
    // TO DO: PAS 3.1: Enviar les propietats del material a la GPU

    // Obtenir localitzadors dels uniforms a la GPU
    GLint locAmbient   = glGetUniformLocation(program, "material.ambient");
    GLint locDiffuse   = glGetUniformLocation(program, "material.diffuse");
    GLint locSpecular  = glGetUniformLocation(program, "material.specular");
    GLint locShininess = glGetUniformLocation(program, "material.shininess");

    // Enviar dades del material si els uniform existeixen
    if (locAmbient != -1)
        glUniform3fv(locAmbient, 1, glm::value_ptr(Ka));

    if (locDiffuse != -1)
        glUniform3fv(locDiffuse, 1, glm::value_ptr(Kd));

    if (locSpecular != -1)
        glUniform3fv(locSpecular, 1, glm::value_ptr(Ks));

    if (locShininess != -1)
        glUniform1f(locShininess, shininess);

}