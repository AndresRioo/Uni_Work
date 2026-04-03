#include "Renders/Raytracing.hpp"

glm::vec3 Raytracing::tracer( Ray ray) const {


    if (config->maxdepth == 0 ){ // hacer raycasting

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

    // sino raytracing
    return tracerRecursive(ray, config->maxdepth + 1 );

}

glm::vec3 Raytracing::tracerRecursive( Ray ray , int depth) const {

    if (depth <= 0) {
        return glm::vec3(0.0f);  // Color negro si llegamos al límite de rebotes ( no ha encontrado una fuente de luz )
    }

    glm::vec3 pixelColor(0.0f);
    vec3 ray2 = normalize(ray.direction);


    ShadeInfo shadeInfo; // vacio hasta que haga un hit    
    bool Ithit = scene->hit(ray, ray.tmin, ray.tmax , shadeInfo);


    // DIBUJAR OBJETO
    if (Ithit){

        vec3 color = shadingStrategy->shading(shadeInfo);

        vec3 color_out;
        Ray ray_out;

        if ( shadeInfo.mat->evaluate(ray , shadeInfo.t, shadeInfo.normal, color_out , ray_out) ) {
            // el material refleja
            //return color + shadeInfo.mat->Ks * tracerRecursive(ray_out,depth - 1 );

            vec3 reflectionColor = tracerRecursive(ray_out, depth - 1);


            // si es transparent
            if(dynamic_pointer_cast<Transp>(shadeInfo.mat)){

                return ( color * ( vec3(1.0f,1.0f,1.0f) - shadeInfo.mat->Kt) )     
                    + 
                    reflectionColor * color_out; // color out siendo kt si refracta (ks si es reflexion interna total)
                

                    // es metalico
            } else {

                // si golpea otro objeto, mirar su reflejo 
                return color + (shadeInfo.mat->Ks * reflectionColor);

            }


        } else { // el material no refleja (lambertian)
            return color;

            
        }       

        
        
        //return shadingStrategy->shading(shadeInfo);
        //return shadeInfo.mat->albedo;
    } else {

        // si no volem que un reflexe retorni el valor de background

        // 1. mirem que la checkbox estigui apagada
        // 2. mirem que el raig actual no sigui el principal
        if ( !config->reflejaElBackground && depth < config->maxdepth ){

            //return lights[0]->getIaGlobal();
            return glm::vec3(0.0f);
        }

        //return lights[0]->getIaGlobal();

        
        
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
}
