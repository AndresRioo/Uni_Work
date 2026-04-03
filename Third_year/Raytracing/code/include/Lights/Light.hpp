#pragma once

#include "glm/glm.hpp"
#include <memory>

using namespace glm;
using namespace std;

class Light {
protected:
    vec3 Ia;
    vec3 Id;
    vec3 Is;
    
    vec3 IaGlobal; // Il·luminació ambiental global


public:
    /*
     * Constructor de la classe Light.
     * param Ia: component ambient de la llum.
     * param Id: component difosa de la llum.
     * param Is: component especular de la llum.
     * */
    Light(vec3 Ia, vec3 Id, vec3 Is): Ia(Ia), Id(Id), Is(Is) {};
    Light(vec3 Ia, vec3 Id, vec3 Is, vec3 IaGlobal) : Ia(Ia), Id(Id), Is(Is), IaGlobal(IaGlobal) {}

    
    Light() {
        this->Ia = vec3(0.01f, 0.01f, 0.01f);
        this->Id = vec3(0.8f, 0.8f, 0.8f);
        this->Is = vec3(1.0f, 1.0f, 1.0f);
    };

    vec3 getIa() { return Ia; };
    vec3 getId() { return Id; };
    vec3 getIs() { return Is; };
    void setIa(vec3 Ia) { this->Ia = Ia; };
    void setId(vec3 Id) { this->Id = Id; };
    void setIs(vec3 Is) { this->Is = Is; };

    vec3 getIaGlobal() { return IaGlobal; }
    void setIaGlobal(vec3 IaGlobal) { this->IaGlobal = IaGlobal; }

    
    //Calcula el factor d'atenuacio de la llum al punt passat per parametre
    virtual float attenuation(vec3 point) = 0;
    //Calcula el vector L amb origen el punt passat per parametre
    virtual vec3 vectorL(vec3 point) = 0;

    //Calcula la distancia del punt a la llum
    virtual float distanceToLight(vec3 point) = 0;

    virtual ~Light() {};
};
