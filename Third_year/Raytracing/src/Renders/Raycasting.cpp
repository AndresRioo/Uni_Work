#include "Renders/Raycasting.hpp"

glm::vec3 Raycasting::tracer( Ray ray) const {
    // Implementació del traçador de rajos
    // TODO: Cal implementar el codi per tenir en compte les interseccions del raig amb l'escena i 
    // retornar el color associat al punt d'intersecció.

    glm::vec3 pixelColor(0.0f);
    vec3 ray2 = normalize(ray.direction);


    ShadeInfo shadeInfo; // vacio hasta que haga un hit    
    bool Ithit = scene->hit(ray, ray.tmin, ray.tmax , shadeInfo);

    // DIBUJAR OBJETO
    if (Ithit){
        return shadingStrategy->shading(shadeInfo);
    }
    

    // DIBUJAR FONDO
    if ( config->backgroundMode ) { // 1 -> textura

        vec2 uv;
        uv.x = 0.5f * (1 + ray2.x); // Convertir [-1,1] a [0,1]
        uv.y = 0.5f * (1 + ray2.y); 

        return config->background->getPixelColor(uv);

    } else { // 0 -> color 

        // devolver background color de config
        return config->backgroundColor; 
    }
}

void Raycasting::setShadingStrategy(shared_ptr<ShadingStrategy> strategy) {
    this->shadingStrategy = strategy;
}

