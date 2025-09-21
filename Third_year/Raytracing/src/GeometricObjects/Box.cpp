#include "GeometricObjects/Box.hpp"

Box::Box() {
    pmin = vec3(-1.0, -1.0, -1.0);
    pmax = vec3(1.0, 1.0, 1.0);
}

Box::Box(vec3 min, vec3 max){
    pmin = min;
    pmax = max;
}

Box::Box(vec3 min, vec3 max, vec3 color){
    pmin = min;
    pmax = max;
    
    material = make_shared<Lambertian>(color);
    material = make_shared<MaterialTextura>(color);
}


/*
bool Box::hit(Ray& r, float tmin, float tmax, ShadeInfo &shadeInfo) const {
    float t_enter = tmin, t_exit = tmax;
    vec3 normal(0, 0, 0);
    float epsilon = 1e-4; // Pequeña tolerancia para errores de punto flotante

    for (int i = 0; i < 3; i++) {
        float invD = 1.0f / r.direction[i];
        float t0 = (pmin[i] - r.origin[i]) * invD;
        float t1 = (pmax[i] - r.origin[i]) * invD;

        if (invD < 0.0f) std::swap(t0, t1);

        if (t0 > t_enter) { 
            t_enter = t0;
            normal = vec3(0, 0, 0); // Reset normal
            normal[i] = -1; // Normal hacia dentro si viene desde fuera
        }
        if (t1 < t_exit) {
            t_exit = t1;
        }

        if (t_exit < t_enter) return false;
    }

    shadeInfo.t = t_enter;
    shadeInfo.p = r(t_enter);
    shadeInfo.mat = material;
    shadeInfo.normal = normal;

    // Determinar la cara de la caja y calcular (u, v)

    return true;
}
*/

bool Box::hit(Ray& r, float tmin, float tmax, ShadeInfo &shadeInfo) const {
    float t_enter = tmin, t_exit = tmax;
    vec3 normal(0, 0, 0);
    float epsilon = 1e-4; // Pequeña tolerancia para errores de punto flotante

    int hitAxis = -1; // Para saber en qué eje ocurre la colisión

    for (int i = 0; i < 3; i++) {
        float invD = 1.0f / r.direction[i];
        float t0 = (pmin[i] - r.origin[i]) * invD;
        float t1 = (pmax[i] - r.origin[i]) * invD;

        if (invD < 0.0f) std::swap(t0, t1);

        if (t0 > t_enter) { 
            t_enter = t0;
            normal = vec3(0, 0, 0);
            normal[i] = (invD < 0) ? 1 : -1; // Normal correcta
            hitAxis = i; // Guardar el eje de colisión
        }
        if (t1 < t_exit) {
            t_exit = t1;
        }

        if (t_exit < t_enter) return false;
    }

    // Guardar datos de la intersección
    shadeInfo.t = t_enter;
    shadeInfo.p = r(t_enter);
    shadeInfo.mat = material;
    shadeInfo.normal = normal;

    // Calcular coordenadas UV según la cara impactada
    vec2 uv(0.0f);
    float u, v;
    vec3 p = shadeInfo.p;

    float minX = pmin.x, maxX = pmax.x;
    float minY = pmin.y, maxY = pmax.y;
    float minZ = pmin.z, maxZ = pmax.z;

    switch (hitAxis) {
        case 0: // Caras izquierda/derecha (X constante)
            u = (p.z - minZ) / (maxZ - minZ);
            v = (p.y - minY) / (maxY - minY);
            break;
        case 1: // Caras arriba/abajo (Y constante)
            u = (p.x - minX) / (maxX - minX);
            v = (p.z - minZ) / (maxZ - minZ);
            break;
        case 2: // Caras delantera/trasera (Z constante)
            u = (p.x - minX) / (maxX - minX);
            v = (p.y - minY) / (maxY - minY);
            break;
    }

    shadeInfo.uv = vec2(u, v);

    return true;
}


bool Box::allHits(Ray& r, float tmin, float tmax, vector<ShadeInfo>& listShadeInfos) const {
    ShadeInfo shadeInfo;
    if (hit(r, tmin, tmax, shadeInfo)) {
        listShadeInfos.push_back(shadeInfo);
        return true;
    }
    return false;
}


void Box::update(int nframe) {
    Animable::update(nframe);
}

void Box::aplicaTG(shared_ptr<TG> tg) {
    if (dynamic_pointer_cast<TranslateTG>(tg)) {
        vec4 min4(pmin, 1.0);
        vec4 max4(pmax, 1.0);
        min4 = tg->getTG() * min4;
        max4 = tg->getTG() * max4;
        pmin = vec3(min4.x, min4.y, min4.z);
        pmax = vec3(max4.x, max4.y, max4.z);
    }
}

vec3 Box::getMinCoords() const {
    return pmin;
}

vec3 Box::getMaxCoords() const {
    return pmax;
}
