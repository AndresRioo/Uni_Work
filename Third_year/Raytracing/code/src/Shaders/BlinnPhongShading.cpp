#include "Shaders/BlinnPhongShading.hpp"

vec3 BlinnPhongShading::shading(ShadeInfo &info) {
    
    vec3 normal = normalize(info.normal); // Normal del punto de intersección

    const auto& light = lights[0];
    vec3 lightDir = normalize(light->vectorL(info.p)); // Obtener dirección de la luz
    float cosTheta = std::max(dot(normal, lightDir), 0.0f); // Calcular coseno del ángulo

    float shadowFactor = 1.0f; // Por defecto, no hay sombra

    if (shadow) { 
        shadowFactor = computeShadow(info.p); // Calcular si hay sombra
    }

    if (!info.mat) { 
        return vec3(0.0f); // Error: no hay material 
    }

    vec3 ambientColor = info.mat->Ka; // Componente ambiental del material
    vec3 diffuseColor = info.mat->getDiffuse(info.uv); // Componente difusa del material
    vec3 especularColor = info.mat->Ks; // Componente especular del material
    float attenuation = light->attenuation(info.p);

    float shininess = info.mat->shininess; // Exponente de reflexión especular
    
    vec3 llumDifosa = diffuseColor * light->getId() * cosTheta; 

    vec3 vectorV = normalize(lookFrom - info.p); // Dirección a la cámara
    vec3 halfVector = normalize(lightDir + vectorV); 

    float specAngle = std::max(dot(normal, halfVector), 0.0f);
    float specFactor = pow(specAngle, shininess); // Reflexión especular

    vec3 llumEspecular = especularColor * light->getIs() * specFactor;

    vec3 llumAmbient = ambientColor * light->getIa();  
    vec3 llumGlobal = ambientColor * light->getIaGlobal();


    // Sumar los componentes de la iluminación
    vec3 totalColor = vec3(0.0f); // Inicializamos el color total

    totalColor += attenuation * llumDifosa * shadowFactor; // component directa, multipliquem per atenuació i per el shadow factor
    totalColor += attenuation * llumEspecular * shadowFactor; // component directa, multipliquem per atenuació i per el shadow factor

    totalColor += llumAmbient ;
    totalColor += llumGlobal ;


    return totalColor;
}



/*
vec3 BlinnPhongShading::shading(ShadeInfo &info) {
    
    vec3 totalColor = vec3(0.0f); // Inicializamos el color total
    vec3 normal = normalize(info.normal); // Normal del punto de intersección

    const auto& light = lights[0];
    vec3 lightDir = normalize(light->vectorL(info.p)); // Obtener dirección de la luz
    float cosTheta = std::max(dot(normal, lightDir), 0.0f); // Calcular coseno del ángulo

    float shadowFactor = 1.0f; // Por defecto, no hay sombra

    if (shadow) { 
        shadowFactor = computeShadow(info.p); // Calcular si hay sombra
    }

    if (!info.mat) { 
        return vec3(0.0f); // error no hay material 
    }

    vec3 ambientColor = info.mat->Ka; //  component ambient del material
    vec3 diffuseColor = info.mat->Kd; //  component difusa del material
    vec3 especularColor = info.mat->Ks; //  component especular

    float shininess = info.mat->shininess; // Exponent de reflexió especular
    vec3 albedo = info.mat->albedo; // color del objeto 
    
    vec3 llumAmbient = ambientColor * light->getIa();  // llum difusa que està implicada en las reflexions indirectes.
    vec3 llumDifosa = diffuseColor * light->getId() *  cosTheta; // llum difusa pura, on la llum es reflexa igualment a totes les direccions 


    totalColor = albedo * llumAmbient * shadowFactor;
    // SOLO LUZ AMBIENTE

    totalColor = totalColor * llumDifosa * shadowFactor;
    // AÑADIR PARTE DIFOSA

    vec3 vectorL = normalize( light->vectorL(info.p) );  // de la interseccion a la luz
    //vec3 vectorV = normalize( -info.ray->direction); // de la interseccion a la camara
    //vec3 vectorV = normalize( this->lookFrom ); // de la interseccion a la camara
    
    vec3 vectorV = normalize(info.ray->origin - info.p);


    //this->lookFrom.

    // Calcular el vector H normalizándolo
    vec3 halfVector = normalize(vectorL + vectorV); 

    float specAngle = std::max(dot(normal, halfVector), 0.0f);
    float specFactor = pow(specAngle, shininess); // Reflexión especular

    vec3 llumEspecular = especularColor * light->getIs() * specFactor;

    totalColor = llumEspecular * shadowFactor;


    //return totalColor;
    // solo especular


    // total
    totalColor = llumAmbient * shadowFactor; // (1) Componente ambiental
    totalColor += llumDifosa * shadowFactor; // (2) Se suma la difusa
    totalColor += llumEspecular * shadowFactor;       // (3) Se suma la especular
    
    return totalColor;


    
    /*
    CODIGO ANTERIOR


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
        //totalColor += info.mat->albedo * cosTheta * attenuation * light->getIa() * light->getIs() * shadowFactor; // Multiplico per crear l'ombra


        //totalColor += info.mat->albedo * cosTheta * attenuation * light->getId() * shadowFactor; 
        // Original Diffuse Shading amb component difosa

        //totalColor += info.mat->albedo * cosTheta * attenuation * shadowFactor * light->getIa(); 
        // COMPONENT AMBIENT 

        totalColor += info.mat->albedo * cosTheta * attenuation * light->getId() * shadowFactor * light->getIa(); 
        // COMPONENT AMBIENT amb difosa

    }

    return totalColor; // Devolver el color total

    //
}

*/


// Funció preparada per manegar més d'una intersecció
vec3 BlinnPhongShading::shading(vector<shared_ptr<ShadeInfo>> infos) {
    if (!infos.empty()) {
        return infos[0]->mat->Kd; // TODO: A modificar per retornar el color en funció de la normal en el punt d'intersecció
    } else {
        // Tracta la situació on el vector infos és buit
        // Retorna un valor predeterminat o maneja-ho segons les teves necessitats.
        return vec3(0.0f);
    }
}

