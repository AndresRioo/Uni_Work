#include "GeometricObjects/Sphere.hpp"

Sphere::Sphere() {
    center = vec3(0.0,0.0,0.0);
    radius = 1.0;
}

Sphere::Sphere(vec3 cen, float r) {
    center = cen;
    radius = r;
}

Sphere::Sphere(vec3 cen, float r, vec3 color) {
    center = cen;
    radius = r;
    material = make_shared<Lambertian>(color);
}


/*
bool Sphere::hit (Ray& raig, float tmin, float tmax, ShadeInfo &shadeInfo) const {
    vec3 oc = raig.origin - center;
    float a = dot(raig.direction, raig.direction);
    float b = dot(oc, raig.direction);
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - a*c;
    if (discriminant > 0) {
        float temp = (-b - sqrt(discriminant))/a;
        if (temp < tmax && temp > tmin) {
            shadeInfo.t = temp;
            shadeInfo.p = raig(temp);
            shadeInfo.normal = (shadeInfo.p - center) / radius;
            shadeInfo.mat = material;


            shadeInfo.ray = std::make_shared<Ray>(raig);


            return true;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < tmax && temp > tmin) {
            shadeInfo.t = temp;
            shadeInfo.p = raig(temp);
            shadeInfo.normal = (shadeInfo.p - center) / radius;
            shadeInfo.mat = material;

            shadeInfo.ray = std::make_shared<Ray>(raig);

            return true;
        }
    }
    return false;
}

*/

bool Sphere::hit(Ray& raig, float tmin, float tmax, ShadeInfo &shadeInfo) const {
    vec3 oc = raig.origin - center;
    float a = dot(raig.direction, raig.direction);
    float b = dot(oc, raig.direction);
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - a * c;

    if (discriminant > 0) {
        float temp = (-b - sqrt(discriminant)) / a;
        if (temp < tmax && temp > tmin) {
            shadeInfo.t = temp;
            shadeInfo.p = raig(temp);
            shadeInfo.normal = (shadeInfo.p - center) / radius;
            shadeInfo.mat = material;
            shadeInfo.ray = std::make_shared<Ray>(raig);

            // Calcular coordenadas UV
            vec3 localPoint = (shadeInfo.p - center) / radius; // Normalizada
            float u = 0.5f + atan2(-localPoint.z, localPoint.x) / (2.0f * M_PI);
            float v = acos(-localPoint.y) / M_PI;
            shadeInfo.uv = vec2(u, v);

            return true;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < tmax && temp > tmin) {
            shadeInfo.t = temp;
            shadeInfo.p = raig(temp);
            shadeInfo.normal = (shadeInfo.p - center) / radius;
            shadeInfo.mat = material;
            shadeInfo.ray = std::make_shared<Ray>(raig);

            // Calcular coordenadas UV
            vec3 localPoint = (shadeInfo.p - center) / radius; // Normalizada
            float u = 0.5f + atan2(-localPoint.z, localPoint.x) / (2.0f * M_PI);
            float v = acos(-localPoint.y) / M_PI;
            shadeInfo.uv = vec2(u, v);

            return true;
        }
    }
    return false;

}


bool Sphere::allHits(Ray& raig, float tmin, float tmax, vector<ShadeInfo>& listShadeInfos) const {

    auto info = make_shared<ShadeInfo>();

    bool trobat = false;
    vec3 oc = raig.origin - center;
    float a = dot(raig.direction, raig.direction);
    float b = dot(oc, raig.direction);
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - a*c;
    if (discriminant > 0) {
        float temp = (-b - sqrt(discriminant))/a;
        if (temp < tmax && temp > tmin) {
            info->t = temp;
            info->p = raig(info->t);
            info->normal = (info->p - center) / radius;
            info->mat = material;
            listShadeInfos.push_back(*info);
            trobat = true;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < tmax && temp > tmin) {
            info->t = temp;
            info->p = raig(info->t);
            info->normal = (info->p - center) / radius;
            info->mat = material;
            listShadeInfos.push_back(*info);
            trobat = true;
        }
    }
    return trobat;
}

void Sphere::update(int frame) {
    // TODO: Cal ampliar-lo per a fer el update de l'esfera
    Animable::update(frame);
}

void Sphere::aplicaTG(shared_ptr<TG> t) {
    if (dynamic_pointer_cast<TranslateTG>(t)) {
        // Per ara només es fan les translacions
        vec4 c(center, 1.0);
        c = t->getTG() * c;
        center.x = c.x; center.y = c.y; center.z = c.z;
    }
    //TODO: Cal ampliar-lo per a acceptar Escalats
}

vec3 Sphere::getMaxCoords() const {
    return vec3(center.x + radius, center.y + radius, center.z + radius);
}

vec3 Sphere::getMinCoords() const {
    return vec3(center.x - radius, center.y - radius, center.z - radius);
}