#include "Shaders/DiffuseShading.hpp"

vec3 DiffuseShading::shading(ShadeInfo &info) {
    vec3 totalColor = vec3(0.0f); // Inicializamos el color total
    vec3 normal = normalize(info.normal); // Normal del punto de intersección

    // Iterar sobre cada luz
    for (const auto& light : lights) {
        vec3 lightDir = normalize(light->vectorL(info.p)); // Obtener dirección de la luz
        float cosTheta = std::max(dot(normal, lightDir), 0.0f); // Calcular coseno del ángulo
        // evitar valores inferiores a 0

        // Calcular la atenuación de la luz ( + lejos = menos luz )
        float attenuation = light->attenuation(info.p); 

        float shadowFactor = 1.0f; // Por defecto, no hay sombra

        if (shadow) { 
            shadowFactor = computeShadow(info.p); // Calcular si hay sombra
        }

        // Acumular el color difuso ponderado por la atenuación y la componente difusa de la luz
        totalColor += info.mat->getDiffuse(info.uv) * cosTheta * attenuation * light->getId() * shadowFactor; // Multiplico per crear l'ombra
    }

    return totalColor; // Devolver el color total
}


// Funció preparada per manegar més d'una intersecció
vec3 DiffuseShading::shading(vector<shared_ptr<ShadeInfo>> infos) {
    if (!infos.empty()) {
        return infos[0]->mat->Kd; // TODO: A modificar per retornar el color en funció de la normal en el punt d'intersecció
    } else {
        // Tracta la situació on el vector infos és buit
        // Retorna un valor predeterminat o maneja-ho segons les teves necessitats.
        return vec3(0.0f);
    }
}

