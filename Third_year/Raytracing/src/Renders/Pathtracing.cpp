#include "Renders/PathTracing.hpp"

glm::vec3 Pathtracing::tracer(Ray ray) const {
    int depth = config->maxdepth + 1; // Evitem que profunditat 0 sigui negra
    return tracerRecursive(ray, depth);
}

glm::vec3 Pathtracing::tracerRecursive(Ray ray, int depth) const {
    if (depth <= 0) {
        return glm::vec3(0.0f);  // Negre si arribem al límit de rebots
    }

    glm::vec3 pixelColor(0.0f);
    vec3 ray2 = normalize(ray.direction);

    ShadeInfo shadeInfo; // Emmagatzema informació de la intersecció
    bool Ithit = scene->hit(ray, ray.tmin, ray.tmax, shadeInfo);

    if (Ithit) {
        // Obtenim el color del shading seleccionat
        vec3 color = shadingStrategy->shading(shadeInfo);

        vec3 color_out;
        std::vector<Ray> rays_out;

        // Avaluem el material per generar rajos secundaris
        if (shadeInfo.mat->evaluate(ray, shadeInfo.t, shadeInfo.normal, color_out, rays_out, config->KpathTracing)) {
            glm::vec3 accumulatedColor(0.0f);

            // Processar tots els rajos generats
            for (const Ray& secondaryRay : rays_out) {
                accumulatedColor += tracerRecursive(secondaryRay, depth - 1);
            }

            // Calcular la mitjana ponderada dels colors
            if (!rays_out.empty()) {
                accumulatedColor /= static_cast<float>(rays_out.size());
            }

            // Si és transparent
            if (dynamic_pointer_cast<Transp>(shadeInfo.mat) && false) {
                return (color * (vec3(1.0f) - shadeInfo.mat->Kt)) + (accumulatedColor * color_out);
            } 
            // Si és metàl·lic o reflexiu
            else if (dynamic_pointer_cast<Metal>(shadeInfo.mat) && false ) {
                return color + (shadeInfo.mat->Ks * accumulatedColor);
            } 

            else if (dynamic_pointer_cast<Lambertian>(shadeInfo.mat)) {
                return accumulatedColor * color_out + color;
            }
        } 
        // Si el material no reflecteix (Lambertian)
        else {
            return color;
        }
    } else {
        // Control de reflexió en el fons
        if (!config->reflejaElBackground && depth <= config->maxdepth) {
            return glm::vec3(0.0f);
        }

        // Dibuixar fons (textura o color)
        if (config->backgroundMode) { // Textura
            vec2 uv;
            uv.x = 0.5f * (1 + ray2.x);
            uv.y = 0.5f * (1 + ray2.y);
            return config->background->getPixelColor(uv);
        } else { // Color de fons simple
            return config->backgroundColor;
        }
    }
}
