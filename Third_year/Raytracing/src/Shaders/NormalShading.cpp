#include "Shaders/NormalShading.hpp"

// Funció per calcular la il·luminació amb un únic punt d'intersecció amb el raig
vec3 NormalShading::shading(ShadeInfo &info) {

    //return  info.mat->Kd; // TODO: A modificar per retornar el color en funció de la normal en el punt d'intersecció

    //vec3 normalizedNormal = normalize(info.normal);     // Normalizamos la normal para asegurarnos de que está en el rango [0, 1]
    //vec3 color = 0.5f * (normalizedNormal + vec3(1.0f)); // Esto convierte [-1,1] a [0,1]

    vec3 color = 0.5f * (info.normal + vec3(1.0f)); // info.normal ya esta normalizado


    if(shadow){

        float ombra = computeShadow(info.p); 
        return color * ombra;  // Atenuar color si está en sombra // Retornem el color basat en la normal
    } else {
        return color; // solo color 
    }
    
}

// Funció preparada per manegar més d'una intersecció
vec3 NormalShading::shading(vector<shared_ptr<ShadeInfo>> infos) {
    if (!infos.empty()) {
        return infos[0]->mat->Kd; // TODO: A modificar per retornar el color en funció de la normal en el punt d'intersecció
    } else {
        // Tracta la situació on el vector infos és buit
        // Retorna un valor predeterminat o maneja-ho segons les teves necessitats.
        return vec3(0.0f);
    }
}

