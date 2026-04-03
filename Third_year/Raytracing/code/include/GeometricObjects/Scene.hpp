#pragma once

#include "GeometricObjects/Object.hpp"
#include "GeometricObjects/Sphere.hpp"
#include "GeometricObjects/Box.hpp"
#include "GeometricObjects/Mesh.hpp"

#include "Utilities/TGs/TranslateTG.hpp"
#include <glm/gtx/component_wise.hpp> // Para glm::compMax y glm::compMin

class Scene: public Hittable, public Animable {
public:

    // Vector d'objectes continguts a l'escena
    vector<shared_ptr<Object>> objects;
    bool useBoundingBoxOptimization = true;


    Scene();
    virtual ~Scene() {};

    // TODO 
    // Funcio que calcula la interseccio del raig r amb l'escena. Guarda la informacio
    // del punt d'interseccio més proper a t_min del Raig, punt que està entre t_min i t_max del Raig.
    // Retorna cert si existeix la interseccio, fals, en cas contrari. A ShadeInfo es retorna la 
    // informació de la intersecció, en cas que existeixi.
    virtual bool hit(Ray& r, float tmin, float tmax, ShadeInfo &shadeInfo) const override;

    // TODO
    // Mètode que retorna totes les interseccions que es troben al llarg del raig entre tmin i tmax
    virtual bool allHits(Ray& r, float tmin, float tmax, vector<ShadeInfo>& listShadeInfos) const override;

    void update(int nframe) override;
    void aplicaTG(shared_ptr<TG> tg) override;

    void addObject (shared_ptr<Object> obj) { objects.push_back(obj);};

    void clear() { objects.clear();};

    void generateRandomSpheres(int numSpheres);

    void init();

    void generateProceduralAnimation(int numFrames);

    // Bounding Box de la escena
    void computeBoundingBox();
    bool boundingBoxHit(Ray& r, float tmin, float tmax, const vec3& invD) const;


    void original();
    void homeworkFitxa2();
    void nomesGranMetalica();
    void totesMetaliques();
    void cubTextura();
    void esferaTextura();
    void esferaTranspTESTING();

    void Fitxa3Test1();
    void Fitxa3Test2();
    void Fitxa3Test3();
    void Fitxa3Test4();

    void testOmbres();


    void ESCENA();
    void ESCENA_BILLAR();

    void pathtracing();

    void AfegirEsferaTextura( vec3 centre , float radi , vec3 color , string archiu , int index );
    void AfegirEsferaMetall(vec3 centre , float radi , vec3 color , int index );
    void AfegirEsferaTransparent(vec3 centre , float radi , vec3 color , int index );

    void AfegirBoxTextura(vec3 origen, vec3 tamany , vec3 color , string archiu, int index);

    void mesh();

private:
    vec3 bboxMin, bboxMax;
};

