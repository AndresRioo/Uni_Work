#include "Materials/MaterialFactory.hpp"

shared_ptr<Material> MaterialFactory::createMaterial(MATERIAL_TYPES t) {
    shared_ptr<Material> m;
    switch (t) {
    case LAMBERTIAN:
        m = make_shared<Lambertian>();
        break;
    case METAL:
        m = make_shared<Metal>();
        break;
    case TRANSP:
        m = make_shared<Transp>();
        break;
    case MATERIALTEXTURA:
        m = make_shared<MaterialTextura>();
        break;
    default:
        break;
    }
    return m;
}

shared_ptr<Material> MaterialFactory::createMaterial(vec3 a, vec3 d, vec3 s, float beta, float opacity, MATERIAL_TYPES t) {
    shared_ptr<Material> m;
    switch (t) {
    case LAMBERTIAN:
        m = make_shared<Lambertian>(a, d, s, beta, opacity);
        break;
    case METAL:
        m = make_shared<Metal>(a, d, s, beta, opacity);
        break;
    case MATERIALTEXTURA:
        m = make_shared<MaterialTextura>(a, d, s, beta, opacity);
        break;
    case TRANSP:
        m = make_shared<Transp>(a, d, s, beta, opacity);
        break;
    default:
        break;
    }
    return m;
}

MaterialFactory::MATERIAL_TYPES MaterialFactory::getIndexType(shared_ptr<Material> m) {
    if (dynamic_pointer_cast<Lambertian>(m) != nullptr) {
        return MATERIAL_TYPES::LAMBERTIAN;
    }
    if (dynamic_pointer_cast<Metal>(m) != nullptr) {
        return MATERIAL_TYPES::METAL;
    }
    if (dynamic_pointer_cast<Transp>(m) != nullptr) {
        return MATERIAL_TYPES::TRANSP;
    }
    if (dynamic_pointer_cast<MaterialTextura>(m) != nullptr) {
        return MATERIAL_TYPES::MATERIALTEXTURA;
    }
    return MATERIAL_TYPES::LAMBERTIAN;
}
