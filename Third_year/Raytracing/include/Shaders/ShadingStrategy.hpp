#pragma once

#include "GeometricObjects/Scene.hpp"
#include "Lights/Light.hpp"
#include "Utilities/ShadeInfo.hpp"

class ShadingStrategy {
protected:
    shared_ptr<Scene> scene;
    vec3 lookFrom;
    bool shadow;
    vector<shared_ptr<Light>> lights;
 
public:
    virtual vec3 shading(ShadeInfo  &info) {
        return vec3(0.0, 0.0, 0.0);
    };

    
    virtual vec3 shading(vector<shared_ptr<ShadeInfo>> info) {
        return vec3(0.0, 0.0, 0.0);
    };
    
    // Operador d'assignació
    ShadingStrategy& operator=(const ShadingStrategy& other) {
        if (this != &other) { // Evitar autoassignació
            scene = other.scene;  // Copiem el shared_ptr (incrementa el comptador de referències)
            lookFrom = other.lookFrom;
            shadow = other.shadow;
            lights = other.lights;
        }
        return *this; // Retornem *this per permetre assignacions encadenades
    }





    // TODO: Calcula si el punt "point" és a l'ombra s
    virtual float computeShadow(vec3 point) {

        float factor_ombra = 1.0f;  // Empezamos con luz total (sin sombra)

        for (const auto& light : lights) {
            vec3 L = normalize(light->vectorL(point));  
            vec3 shadowOrigin = point + L * 0.01f;  // Para evitar problemas de precisión
            Ray shadowRay(shadowOrigin, L);  

            std::vector<ShadeInfo> shadowInfos;

            // Obtener todas las intersecciones con objetos
            if (scene->allHits(shadowRay, 0.001f, light->distanceToLight(point), shadowInfos)) {
                for (const auto& shadeInfo : shadowInfos) {
                    // Comprobamos si el material es de tipo Transp (material transparente)
                    if (auto transpMat = std::dynamic_pointer_cast<Transp>(shadeInfo.mat)) {
                        float d = shadeInfo.t;  // Distancia recorrida dentro del objeto
                        float dmax = transpMat->getDmax();  // Llamamos a getDmax() sobre el objeto Transp

                        // Aplicamos la fórmula de acumulación de opacidades
                        factor_ombra *= (1.0f - (d / dmax));

                        // Si la opacidad acumulada es 0, la luz no pasa más
                        if (factor_ombra <= 0.0f) {
                            return 0.0f;
                        }
                    } else {
                        // Si encontramos un objeto opaco, sombra total
                        return 0.0f;
                    }
                }
            }
        }

        return factor_ombra;  // Devuelve el factor de sombra acumulado


        /*
        COMPUTE SHADOW ANTERIOR

        for (const auto& light : lights) {  
            
            vec3 L = normalize(light->vectorL(point));  
            vec3 shadowOrigin = point + L * 0.001f; 
            
            Ray shadowRay(shadowOrigin, L); 
            ShadeInfo shadowInfo;
  
            // Si NO hay intersección con un objeto antes de la luz, hay iluminación
            if (!scene->hit(shadowRay, 0.001f, light->distanceToLight(point), shadowInfo)) {
                return 1.0f;  // Punto iluminado
            }
          }
  
        return 0.0f;  // Punto en sombra
      }
        */
    };
};
/*

float computeShadow(vec3 point) {

    vec3 lightDir = normalize(lights[0]->vectorL(point)); 

    Ray shadowRay(point + lightDir * 0.001f, lightDir);
    std::vector<ShadeInfo> intersections;


    bool hitSomething = scene->allHits(shadowRay, 0.001f, FLT_MAX, intersections);
    
    if (!hitSomething) return 1.0f; // No hay objetos → luz completa

    float transparencyTotal = 1.0f; // Factor de transmisión de luz

    for (size_t i = 0; i < intersections.size(); i += 2) {
        if (i + 1 < intersections.size()) {
            ShadeInfo entry = intersections[i];
            ShadeInfo exit = intersections[i + 1];

            float dj = exit.t - entry.t;  // Distancia recorrida en el material
            float dmax = entry.mat->dmax; // Distancia para opacidad total

            float alpha = glm::clamp(dj / dmax, 0.0f, 1.0f); // Opacidad del material
            transparencyTotal *= (1.0f - alpha); // Acumulación de transparencia

            if (transparencyTotal <= 0.01f) return 0.0f; // Se vuelve opaco
        }
    }

    return glm::clamp(transparencyTotal, 0.0f, 1.0f);
}

virtual ~ShadingStrategy() {};
};
*/
