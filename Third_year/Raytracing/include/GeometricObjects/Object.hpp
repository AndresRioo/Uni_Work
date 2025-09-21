#pragma once

#include "Utilities/Hittable.hpp"
#include "Utilities/Animable.hpp"

#include "Materials/Material.hpp"
#include "Materials/Lambertian.hpp"
#include "Materials/Metal.hpp"
#include "Materials/Transp.hpp"
#include "Materials/MaterialTextura.hpp"


using namespace std;

// Es la classe base de tots els objectes a renderitzar
// implementa dues interficies: Hittable i Animable

class Object: public Hittable, public Animable {
  public:
    Object();
    virtual ~Object() {};

    // Metodes a implementar en les classes filles: son  metodes abstractes
    virtual bool hit (Ray& r, float tmin, float tmax, ShadeInfo &shadeInfo) const override = 0;
    virtual bool allHits(Ray& r, float tmin, float tmax, vector<ShadeInfo>& listShadeInfos) const override = 0;

    virtual void update(int nframe) override = 0;
    virtual void aplicaTG(shared_ptr<TG>) override = 0 ;
    
    void                 setMaterial(shared_ptr<Material> m);
    shared_ptr<Material> getMaterial();

    // Métodos virtuales puros para obtener los límites del objeto
    virtual vec3 getMinCoords() const = 0;
    virtual vec3 getMaxCoords() const = 0;

protected:
    shared_ptr<Material> material;   // Material de l'objecte
};
