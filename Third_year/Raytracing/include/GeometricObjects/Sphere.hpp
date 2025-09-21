/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#pragma once

#include "Object.hpp"
#include "Utilities/TGs/TG.hpp"
#include "Utilities/TGs/TranslateTG.hpp"

class Sphere: public Object  {
public:
    Sphere();

    Sphere(vec3 cen, float r);
    Sphere(vec3 cen, float r, vec3 col);

    virtual ~Sphere() {}
    virtual bool hit (Ray& r, float tmin, float tmax, ShadeInfo &shadeInfo) const override;
    virtual bool allHits(Ray& r, float tmin, float tmax, vector<ShadeInfo>& listShadeInfos) const override;

    virtual void update(int nframe) override;
    virtual void aplicaTG(shared_ptr<TG> tg) override;


    vec3  getCenter() { return center;};
    float getRadius() { return radius;}

    vec3 getMaxCoords() const;
    vec3 getMinCoords() const;

private:
    // Centre de l'esfera
    vec3 center;
    // Radi de l'esfera
    float radius;
};
