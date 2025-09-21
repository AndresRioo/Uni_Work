/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#pragma once

#include "Object.hpp"
#include "Utilities/TGs/TG.hpp"
#include "Utilities/TGs/TranslateTG.hpp"
#include <glm/glm.hpp>


class Box : public Object {
public:
    Box();
    Box(vec3 min, vec3 max);
    Box(vec3 min, vec3 max, vec3 color);

    virtual ~Box() {}
    virtual bool hit(Ray& r, float tmin, float tmax, ShadeInfo &shadeInfo) const override;
    virtual bool allHits(Ray& r, float tmin, float tmax, vector<ShadeInfo>& listShadeInfos) const override;
    
    virtual void update(int nframe) override;
    virtual void aplicaTG(shared_ptr<TG> tg) override;

    vec3 getMinCoords() const;
    vec3 getMaxCoords() const;

private:
    vec3 pmin, pmax;
};
