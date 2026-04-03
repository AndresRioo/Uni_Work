#include "GeometricObjects/Scene.hpp"
#include <iostream>


#include <filesystem>
#include <iostream>

#include <algorithm>  // Necesario para std::sort


Scene::Scene()
{
    std::cout << "Constructor de Scene" << std::endl;
    init();
}


bool Scene::hit(Ray& r, float tmin, float tmax, ShadeInfo &shadeInfo) const {
    // TODO TUTORIAL 0 i TUTORIAL 1:
    // Heu de codificar la vostra solucio per aquest metode substituint el 'return false'
    // Una possible solucio es cridar el mètode "hit" per a tots els objectes i quedar-se amb la interseccio
    // mes propera a l'observador entre tmin i tmax, en el cas que n'hi hagi més d'una.
    // Si un objecte es intersecat pel raig, cal actualitzar la variable shadeInfo 


    
    if (useBoundingBoxOptimization) {
        vec3 invD = vec3(1.0f) / r.direction; // Precomputamos para evitar cálculos repetidos
        if (!boundingBoxHit(r, tmin, tmax, invD))
            return false;
    }

    bool hitSomething = false;
    float closestT = tmax;

    for (const auto& obj : objects) {
        if (obj->hit(r, tmin, closestT, shadeInfo)) {
            hitSomething = true;
            closestT = shadeInfo.t; // Ajustamos el rango de búsqueda
        }
    }
    return hitSomething;
    

    
    ///*METODO ANTERIOR
    //return objects[0]->hit(r, tmin, tmax, shadeInfo); solución solo para el primer objeto

    bool hit = false;
    shadeInfo.t = tmax; // Inicializar shadeInfo.t para la comparación inicial

    for (const auto& object : objects) {

        ShadeInfo shadeIndividual = ShadeInfo(); // variable vacia para ir guardando todos los hits y comparar con el mínimo encontrado

        if (object->hit(r, tmin, tmax, shadeIndividual)) {
            // guardar en shadeIndividual la información del hit ( si ha habido )

            hit = true; // existe 1 intersección
            
            if ( shadeInfo.t > shadeIndividual.t ){ 
                // Actualizar la interseccion mínima

                shadeInfo.operator=(shadeIndividual);
            }
        }

        //break; // para 1 único objeto
    }

    return hit;  // existe 1 intersección
    //*/

}

bool Scene::allHits(Ray& r, float tmin, float tmax, vector<ShadeInfo>& listShadeInfos) const {
    // TODO TUTORIAL 0 i TUTORIAL 1:
    // Heu de codificar la vostra solucio per aquest metode substituint el 'return false'
    // Una possible solucio es cridar el mètode "hit" per a tots els objectes i quedar-se amb totes
    // les interseccions
    // Si un objecte es intersecat pel raig entre tmin i tmax, el parametre  de tipus ShadeInfo conte
    // la informació sobre la interseccio.
    // Cada vegada que s'intersecta un objecte s'ha d'afegir un nou ShadeInfo a la llista de ShadeInfos


    /*
    for (const auto& object : this->objects ) {
        
        if (    object->getMaterial().(r,)          )        {
            object->getMaterial()->albedo;
        }

        break; // solo el primero
    }
    */

    bool hasHit = false;

    for (const auto& object : this->objects) {
        ShadeInfo shadeInfo;
        if (object->hit(r, tmin, tmax, shadeInfo)) {
            listShadeInfos.push_back(shadeInfo);
            hasHit = true;
        }
    }

    // Ordenar las intersecciones por distancia (para procesarlas en orden)
    std::sort(listShadeInfos.begin(), listShadeInfos.end(), 
              [](const ShadeInfo& a, const ShadeInfo& b) {
                  return a.t < b.t;
              });

    return hasHit;
}

void Scene::update(int nframe) {
    for (unsigned int i = 0; i< objects.size(); i++) {
        objects[i]->update(nframe);
    }
}

void Scene::aplicaTG(shared_ptr<TG> tg) {
    // TODO
}

void Scene::init() {

    objects.clear();

    //testOmbres();

    //original();
    //homeworkFitxa2();
    //nomesGranMetalica();
    //totesMetaliques();
    //cubTextura();
    //esferaTextura();
    //esferaTranspTESTING();

    //Fitxa3Test1();
    Fitxa3Test2();
    //Fitxa3Test3();
    //Fitxa3Test4();

    //ESCENA();
    //ESCENA_BILLAR();

    //pathtracing();

    //mesh();

    return;


}







































































void Scene::generateRandomSpheres(int numSpheres) {
    // Pots modificar aquest mètode com vulguis per a generar les esferes que vulguis
    objects.clear(); // Neteja els objectes anteriors

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> posDist(-5.0f, 5.0f); // Distribució per posició
    std::uniform_real_distribution<float> radDist(0.5f, 1.5f); // Distribució per radi
    std::uniform_real_distribution<float> colDist(0.0f, 1.0f); // Distribució per color

    for (int i = 0; i < numSpheres; ++i) {
        glm::vec3 center(posDist(gen), posDist(gen), posDist(gen) - 4.0f);
        float radius = radDist(gen);
        glm::vec3 color(colDist(gen), colDist(gen), colDist(gen));

        objects.push_back(std::make_shared<Sphere>(center, radius, color));
    }
    objects.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, -100.5f, -5.0f), 95.0f, glm::vec3(0.8f, 0.8f, 0.0f))); // Esfera gran

    /*
    for (auto& obj : objects) {
        obj->setMaterial(std::make_shared<Metal>(obj->getMaterial()->albedo));
    }
    */

    computeBoundingBox();
 
}

void Scene::generateProceduralAnimation(int numFrames) {
    float tx = 0.1f;
    float ty = 0.05f;
    float tz = 0.0f;
    for (auto o: objects) { 
        glm::vec3 t = glm::vec3(tx, ty, tz);
        auto animation = make_shared<Animation>();
        animation->frameIni = 0;
        animation->frameFinal = numFrames;
        animation->transf = make_shared<TranslateTG>(t);
        o->addAnimation(animation);
    }

    // TODO: fer les animacions que vulguis amb els teus objectes
}

void Scene::computeBoundingBox() {
    if (objects.empty()) {
        bboxMin = vec3(0);
        bboxMax = vec3(0);
        return;
    }

    // Inicializamos con los valores del primer objeto
    bboxMin = objects[0]->getMinCoords();
    bboxMax = objects[0]->getMaxCoords();

    for (const auto& obj : objects) {
        vec3 objMin = obj->getMinCoords();
        vec3 objMax = obj->getMaxCoords();

        bboxMin = vec3(fmin(bboxMin.x, objMin.x), fmin(bboxMin.y, objMin.y), fmin(bboxMin.z, objMin.z));
        bboxMax = vec3(fmax(bboxMax.x, objMax.x), fmax(bboxMax.y, objMax.y), fmax(bboxMax.z, objMax.z));
    }

    //objects.push_back(std::make_shared<Box>(bboxMin,bboxMax, vec3(1.0f, 1.0f, 1.0f)));

}

bool Scene::boundingBoxHit(Ray& r, float tmin, float tmax, const vec3& invD) const {
    vec3 t0s = (bboxMin - r.origin) * invD;
    vec3 t1s = (bboxMax - r.origin) * invD;

    vec3 tminVec = glm::min(t0s, t1s);
    vec3 tmaxVec = glm::max(t0s, t1s);

    float tNear = glm::compMax(tminVec);
    float tFar = glm::compMin(tmaxVec);

    return tNear <= tFar && tFar >= tmin && tNear <= tmax;
}




void Scene::original(){

    objects.clear();

    objects.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, -5.0f), 1.0f, glm::vec3(1.0f, 0.0f, 0.0f)));  // roja
    objects.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, -100.5f, -5.0f), 100.0f, glm::vec3(0.8f, 0.8f, 0.0f))); // amarilla grande
    objects.push_back(std::make_shared<Sphere>(glm::vec3(2.0f, 1.0f, -6.0f), 1.5f, glm::vec3(0.0f, 1.0f, 0.0f)));  // verde
    objects.push_back(std::make_shared<Sphere>(glm::vec3(-2.0f, 1.0f, -4.0f), 1.0f, glm::vec3(0.0f, 0.0f, 1.0f)));  // azul

    computeBoundingBox();

}



void Scene::homeworkFitxa2() {

    objects.clear();

    // Generar una posición aleatoria para la esfera
    float posX = glm::linearRand(0.0f, 5.0f); // Rango para la posición X
    float posY = glm::linearRand(0.0f, 5.0f); // Rango para la posición Y
    float posZ = glm::linearRand(0.0f, -2.0f); // Rango para la posición Z

    // Generar un radio aleatorio para la esfera
    float radio = glm::linearRand(0.5f, 2.0f); // Rango para el radio

    // Crear la esfera con posición y radio aleatorios
    objects.push_back(std::make_shared<Sphere>(glm::vec3(posX, posY, posZ), radio, glm::vec3(1.0f, 0.0f, 0.0f))); // Esfera vermella

    vec3 color1(glm::linearRand(0.0, 1.0), glm::linearRand(0.0, 1.0),glm::linearRand(0.0, 1.0));
    vec3 color2(glm::linearRand(0.0, 1.0), glm::linearRand(0.0, 1.0),glm::linearRand(0.0, 1.0));
    vec3 Kd = color1*color2;
    vec3 Ks(glm::linearRand(0.7, 1.0), glm::linearRand(0.7, 1.0), glm::linearRand(0.7, 1.0));
    vec3 Ka(glm::linearRand(0.7, 1.0), glm::linearRand(0.7, 1.0), glm::linearRand(0.7, 1.0));

    auto materialRandom = std::make_shared<Metal>();

    materialRandom->Ka = Ka; // component ambient
    materialRandom->Kd = Kd; // component difusa
    materialRandom->Ks = Ks; // component especular

    materialRandom->shininess = glm::linearRand(10.0f, 100.0f); 

    objects[0]->setMaterial(materialRandom); 

    objects.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, -5.0f), 1.0f, glm::vec3(1.0f, 0.0f, 0.0f)));  // roja
    objects.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, -100.5f, -5.0f), 100.0f, glm::vec3(0.8f, 0.8f, 0.0f))); // amarilla grande
    objects.push_back(std::make_shared<Sphere>(glm::vec3(2.0f, 1.0f, -6.0f), 1.5f, glm::vec3(0.0f, 1.0f, 0.0f)));  // verde
    objects.push_back(std::make_shared<Sphere>(glm::vec3(-2.0f, 1.0f, -4.0f), 1.0f, glm::vec3(0.0f, 0.0f, 1.0f)));  // azul
    


    computeBoundingBox();
}

void Scene::nomesGranMetalica(){

    objects.clear();

    objects.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, -5.0f), 1.0f, glm::vec3(1.0f, 0.0f, 0.0f))); // Esfera vermella
    objects.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, -100.5f, -5.0f), 100.0f, glm::vec3(0.8f, 0.8f, 0.0f) )); // Esfera gran groguenca
    
    
    // la bola grande de metal
    auto material = std::make_shared<Metal>(glm::vec3(0.8f, 0.8f, 0.0f));

    material->Ka = vec3(0.2f,0.2f,0.2f); // component ambient
    material->Kd = vec3(0.8f,0.8f,0.0f); // component difusa
    material->Ks = vec3(1.0f,1.0f, 1.0f); // component especular

    material->shininess = 100.0f; 

    objects[1]->setMaterial(material); 
    
    objects.push_back(std::make_shared<Sphere>(glm::vec3(2.0f, 1.0f, -6.0f), 1.5f, glm::vec3(0.0f, 1.0f, 0.0f))); // Esfera verda
    objects.push_back(std::make_shared<Sphere>(glm::vec3(-2.0f, 1.0f, -4.0f), 1.0f, glm::vec3(0.0f, 0.0f, 1.0f))); // Esfera blava

    computeBoundingBox();

}


void Scene::totesMetaliques(){

    objects.clear();

    objects.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, -5.0f), 1.0f, glm::vec3(1.0f, 0.0f, 0.0f))); // Esfera vermella
    objects.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, -100.5f, -5.0f), 100.0f, glm::vec3(0.8f, 0.8f, 0.0f) )); // Esfera gran groguenca    
    objects.push_back(std::make_shared<Sphere>(glm::vec3(2.0f, 1.0f, -6.0f), 1.5f, glm::vec3(0.0f, 1.0f, 0.0f))); // Esfera verda
    objects.push_back(std::make_shared<Sphere>(glm::vec3(-2.0f, 1.0f, -4.0f), 1.0f, glm::vec3(0.0f, 0.0f, 1.0f))); // Esfera blava

    // material metàl·lic
    //auto material = std::make_shared<Metal>(glm::vec3(0.8f, 0.8f, 0.0f));

    //material->albedo

    //material->Ka = vec3(0.2f,0.2f,0.2f); // component ambient
    //material->Kd = vec3(0.8f,0.8f,0.0f); // component difusa
    //material->Ks = vec3(1.0f,1.0f,1.0f); // component especular

    //material->shininess = 100.0f; 

    for (auto& obj : objects) {
        obj->setMaterial( std::make_shared<Metal>(glm::vec3(obj->getMaterial()->albedo)));
    }

    computeBoundingBox();

}


void Scene::cubTextura(){

    objects.clear();

    // BOX AMB TEXTURE

    // Crea una caja en el origen (0, 0, 0) con tamaño (1, 1, 1) y un color base
    objects.push_back(std::make_shared<Box>(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.3f, 0.0f, 0.2f))); 

    glm::vec3 color1 = glm::vec3(0.8f, 0.8f, 0.8f);

    // Obtiene la ruta base del proyecto
    std::filesystem::path base_path = std::filesystem::current_path();
    base_path = base_path.parent_path();  // Retrocede un nivel en la jerarquía de directorios (estamos en build)

    // Combina la ruta base con la ruta de la textura
    std::filesystem::path texture_path = base_path / "resources" / "desert.jpg";

    //std::string absolute_texture_path = "C:\\Users\\rioan\\OneDrive\\Documents\\3eroooo de UNI\\Segundo Cuatri\\Grafics\\Practica 1\\p1-pathtracingtoy-b08\\resources\\pintura.jpg";

    /*
    if (!std::filesystem::exists(absolute_texture_path)) {
        std::cerr << "El archivo no existe: " << texture_path << std::endl;
    } else {
        std::cout << "Archivo encontrado: " << texture_path << std::endl;
    }
    */

    if (!std::filesystem::exists(texture_path)) {
        std::cerr << "El archivo no existe: " << texture_path << std::endl;
    } else {
        std::cout << "Archivo encontrado: " << texture_path << std::endl;
    }

    

    // Crea un material de textura con el color base
    auto materialTextura = std::make_shared<MaterialTextura>(color1);

    // Crea un objeto Image utilizando la ruta de la textura y asegúrate de que sea el tipo correcto
    materialTextura->background = std::make_shared<Image>(texture_path.string().c_str()); 

    // Asigna el material a la caja 
    objects[0]->setMaterial(materialTextura); 

    computeBoundingBox();
}

void Scene::esferaTextura(){

    // Generar un radio aleatorio para la esfera
    float radio = 2.3f; // Rango para el radio

    //objects.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, 0.0f), radio, glm::vec3(0.3f, 0.0f, 0.2f))); // Esfera vermella
    objects.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, -5.0f), radio, glm::vec3(0.3f, 0.0f, 0.2f))); // Esfera vermella

    glm::vec3 color_bola = glm::vec3(0.8f, 0.8f, 0.8f);

    // Obtiene la ruta base del proyecto
    std::filesystem::path base_path_bola = std::filesystem::current_path();
    base_path_bola = base_path_bola.parent_path();  // Retrocede un nivel en la jerarquía de directorios (estamos en build)



    // Combina la ruta base con la ruta de la textura
    std::filesystem::path texture_path_bola = base_path_bola / "resources" / "pintura.jpg";

    //std::string absolute_texture_path_bola = "C:\\Users\\rioan\\OneDrive\\Documents\\3eroooo de UNI\\Segundo Cuatri\\Grafics\\Practica 1\\Capturas\\emojiPng.png";
    
    /*
    std::string absolute_texture_path_bola = "C:\\Users\\rioan\\OneDrive\\Documents\\3eroooo de UNI\\Segundo Cuatri\\Grafics\\Practica 1\\p1-pathtracingtoy-b08\\resources\\desert.jpg";

    if (!std::filesystem::exists(absolute_texture_path_bola)) {
        std::cerr << "El archivo no existe: " << absolute_texture_path_bola << std::endl;
    } else {
        std::cout << "Archivo encontrado: " << absolute_texture_path_bola << std::endl;
    }
    */

    if (!std::filesystem::exists(texture_path_bola)) {
        std::cerr << "El archivo no existe: " << texture_path_bola << std::endl;
    } else {
        std::cout << "Archivo encontrado: " << texture_path_bola << std::endl;
    }
        

    // Crea un material de textura con el color base
    auto materialTexturaBola = std::make_shared<MaterialTextura>(color_bola);

    // Crea un objeto Image utilizando la ruta de la textura y asegúrate de que sea el tipo correcto
    //materialTexturaBola->background = std::make_shared<Image>(absolute_texture_path_bola.c_str()); 
    materialTexturaBola->background = std::make_shared<Image>(texture_path_bola.string().c_str()); 

    // Asigna el material a la caja 
    objects[0]->setMaterial(materialTexturaBola); 

    computeBoundingBox();
}


void Scene::esferaTranspTESTING(){


    // Escenario original con todo metalico

    objects.clear(); // Limpiar la lista de objetos antes de añadir nuevos
    //objects.push_back(std::make_shared<Box>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
    
    objects.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, -5.0f), 1.0f, glm::vec3(1.0f, 0.0f, 0.0f))); 


    auto materialPrueba = std::make_shared<Metal>(glm::vec3(1.0f, 0.0f, 0.0f));


    materialPrueba->Ka = vec3(0.2f,0.2f,0.2f); // component ambient
    materialPrueba->Kd = vec3(0.8f,0.8f,0.0f); // component difusa
    materialPrueba->Ks = vec3(1.0f,1.0f,1.0f); // component especular

    materialPrueba->shininess = 100.0f;

    objects.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, -100.5f, -5.0f), 100.0f, glm::vec3(0.8f, 0.8f, 0.0f))); 
    objects.push_back(std::make_shared<Sphere>(glm::vec3(2.0f, 1.0f, -6.0f), 1.5f, glm::vec3(0.0f, 1.0f, 0.0f))); 
    objects.push_back(std::make_shared<Sphere>(glm::vec3(-2.0f, 1.0f, -4.0f), 1.0f, glm::vec3(0.0f, 0.0f, 1.0f))); 
    

    
    for (auto& obj : objects) {
        obj->setMaterial(std::make_shared<Metal>(obj->getMaterial()->albedo));
        //obj->setMaterial(materialPrueba);
    }

    // Esfera transparente

    glm::vec3 colorG = glm::vec3(1.0f, 0.0f, 0.0f);

    // la bola grande de metal
    auto materialTransparente = std::make_shared<Transp>(colorG);

    materialTransparente->Ka = vec3(0.2f,0.2f,0.2f); // component ambient
    materialTransparente->Kd = vec3(0.8f,0.8f,0.0f); // component difusa
    materialTransparente->Ks = vec3(1.0f,1.0f,1.0f); // component especular

    materialTransparente->Kt = vec3(0.7f,0.7f,0.7f); // component transparent?


    materialTransparente->shininess = 10.0f; 
    materialTransparente->opacity= 0.3f; // LO CONTRARIO A KT
    materialTransparente->ior= 1.0f; 

    objects[3]->setMaterial(materialTransparente); 

    computeBoundingBox();

}


void Scene::Fitxa3Test1(){

    objects.clear();

    // FILA 1 

    // bola grande de abajo

    glm::vec3 centre = glm::vec3(0.0f, -10004.0f , -20.0f );
    float radio = 10000.0f; 
    glm::vec3 color = glm::vec3(0.5f, 0.5f , 0.5f );

    objects.push_back(std::make_shared<Sphere>(centre, radio, color)); // Esfera vermella

    auto materialFila1 = std::make_shared<Lambertian>(color);

    materialFila1->Ka = glm::vec3(0.2f,0.2f,0.2f);
    materialFila1->Kd = glm::vec3(0.2f,0.2f,0.2f);
    materialFila1->Ks = glm::vec3(1.0f,1.0f,1.0f);

    // OPACIDAD = 1 - KT

    materialFila1->Kt = vec3(0.0f,0.0f,0.0f); // component transparent?
    materialFila1->opacity= 1.0f; // LO CONTRARIO A KT

    materialFila1->shininess = 1.0f; // BETA
    materialFila1->ior= 1.0f; 

    objects[0]->setMaterial(materialFila1); 



    // FILA 2 

    // bola pequeña en el centro
    glm::vec3 centre2 = glm::vec3(0.0f, 0.0f, -20.0f);
    float radio2 = 4.0f; 
    glm::vec3 color2 = glm::vec3(1.0f, 0.32f, 0.36f);

    objects.push_back(std::make_shared<Sphere>(centre2, radio2, color2)); // Esfera roja

    auto materialFila2 = std::make_shared<Transp>(color2);

    materialFila2->Ka = glm::vec3(0.2f, 0.2f, 0.2f);
    materialFila2->Kd = glm::vec3(0.8f, 0.32f, 0.36f);
    materialFila2->Ks = glm::vec3(1.0f, 1.0f, 1.0f);

    materialFila2->Kt = vec3(0.5f, 0.5f, 0.5f); // componente transparente
    materialFila2->opacity = 0.5f; // LO CONTRARIO A KT

    materialFila2->shininess = 10.0f; // BETA
    materialFila2->ior = 1.0f; 

    objects[1]->setMaterial(materialFila2);

    // FILA 3 

    // bola intermedia
    glm::vec3 centre3 = glm::vec3(5.0f, -1.0f, -15.0f);
    float radio3 = 2.0f; 
    glm::vec3 color3 = glm::vec3(0.90f, 0.76f, 0.46f);

    objects.push_back(std::make_shared<Sphere>(centre3, radio3, color3)); // Esfera amarilla

    auto materialFila3 = std::make_shared<Lambertian>(color3);

    materialFila3->Ka = glm::vec3(0.2f, 0.2f, 0.2f);
    materialFila3->Kd = glm::vec3(0.90f, 0.76f, 0.46f);
    materialFila3->Ks = glm::vec3(1.0f, 1.0f, 1.0f);

    materialFila3->Kt = vec3(1.0f, 1.0f, 1.0f); // componente transparente
    materialFila3->opacity = 0.0f; // LO CONTRARIO A KT

    materialFila3->shininess = 10.0f; // BETA
    materialFila3->ior = 1.0f; 

    objects[2]->setMaterial(materialFila3);

    // FILA 4 

    // otra bola
    glm::vec3 centre4 = glm::vec3(5.0f, 0.0f, -25.0f);
    float radio4 = 3.0f; 
    glm::vec3 color4 = glm::vec3(0.65f, 0.77f, 0.97f);

    objects.push_back(std::make_shared<Sphere>(centre4, radio4, color4)); // Esfera azul

    auto materialFila4 = std::make_shared<Lambertian>(color4);

    materialFila4->Ka = glm::vec3(0.2f, 0.2f, 0.2f);
    materialFila4->Kd = glm::vec3(0.65f, 0.77f, 0.97f);
    materialFila4->Ks = glm::vec3(1.0f, 1.0f, 1.0f);

    materialFila4->Kt = vec3(1.0f, 1.0f, 1.0f); // componente transparente
    materialFila4->opacity = 0.0f; // LO CONTRARIO A KT

    materialFila4->shininess = 10.0f; // BETA
    materialFila4->ior = 1.0f; 

    objects[3]->setMaterial(materialFila4);

    // FILA 5 

    // bola gris
    glm::vec3 centre5 = glm::vec3(-5.5f, 0.0f, -15.0f);
    float radio5 = 3.0f; 
    glm::vec3 color5 = glm::vec3(0.90f, 0.90f, 0.90f);

    objects.push_back(std::make_shared<Sphere>(centre5, radio5, color5)); // Esfera gris

    auto materialFila5 = std::make_shared<Lambertian>(color5);

    materialFila5->Ka = glm::vec3(0.2f, 0.2f, 0.2f);
    materialFila5->Kd = glm::vec3(0.90f, 0.90f, 0.90f);
    materialFila5->Ks = glm::vec3(1.0f, 1.0f, 1.0f);

    materialFila5->Kt = vec3(1.0f, 1.0f, 1.0f); // componente transparente
    materialFila5->opacity = 0.0f; // LO CONTRARIO A KT

    materialFila5->shininess = 10.0f; // BETA
    materialFila5->ior = 1.0f; 

    objects[4]->setMaterial(materialFila5);





    computeBoundingBox();



}


void Scene::Fitxa3Test2(){


    objects.clear();

    // FILA 1 

    // bola grande de abajo

    glm::vec3 centre = glm::vec3(0.0f, -10004.0f , -20.0f );
    float radio = 10000.0f; 
    glm::vec3 color = glm::vec3(0.5f, 0.5f , 0.5f );

    objects.push_back(std::make_shared<Sphere>(centre, radio, color)); // Esfera vermella

    auto materialFila1 = std::make_shared<Lambertian>(color);

    materialFila1->Ka = glm::vec3(0.2f,0.2f,0.2f);
    materialFila1->Kd = glm::vec3(0.2f,0.2f,0.2f);
    materialFila1->Ks = glm::vec3(1.0f,1.0f,1.0f);

    // OPACIDAD = 1 - KT

    materialFila1->Kt = vec3(0.0f,0.0f,0.0f); // component transparent?
    materialFila1->opacity= 1.0f; // LO CONTRARIO A KT

    materialFila1->shininess = 1.0f; // BETA
    materialFila1->ior= 1.0f; 

    objects[0]->setMaterial(materialFila1); 



    // FILA 2 

    // bola pequeña en el centro
    glm::vec3 centre2 = glm::vec3(0.0f, 0.0f, -20.0f);
    float radio2 = 4.0f; 
    glm::vec3 color2 = glm::vec3(1.0f, 0.32f, 0.36f);

    objects.push_back(std::make_shared<Sphere>(centre2, radio2, color2)); // Esfera roja

    auto materialFila2 = std::make_shared<Transp>(color2);

    materialFila2->Ka = glm::vec3(0.2f, 0.2f, 0.2f);
    materialFila2->Kd = glm::vec3(0.8f, 0.32f, 0.36f);
    materialFila2->Ks = glm::vec3(1.0f, 1.0f, 1.0f);

    materialFila2->Kt = vec3(0.75f, 0.75f, 0.75f); // componente transparente
    materialFila2->opacity = 0.25f; // LO CONTRARIO A KT

    materialFila2->shininess = 10.0f; // BETA
    materialFila2->ior = 1.0f; 

    objects[1]->setMaterial(materialFila2);

    // FILA 3 

    // bola intermedia
    glm::vec3 centre3 = glm::vec3(5.0f, -1.0f, -15.0f);
    float radio3 = 2.0f; 
    glm::vec3 color3 = glm::vec3(0.90f, 0.76f, 0.46f);

    objects.push_back(std::make_shared<Sphere>(centre3, radio3, color3)); // Esfera amarilla

    auto materialFila3 = std::make_shared<Metal>(color3);

    materialFila3->Ka = glm::vec3(0.2f, 0.2f, 0.2f);
    materialFila3->Kd = glm::vec3(0.90f, 0.76f, 0.46f);
    materialFila3->Ks = glm::vec3(0.7f, 0.7f, 0.7f);

    materialFila3->Kt = vec3(1.0f, 1.0f, 1.0f); // componente transparente
    materialFila3->opacity = 0.0f; // LO CONTRARIO A KT

    materialFila3->shininess = 10.0f; // BETA
    materialFila3->ior = 1.0f; 

    objects[2]->setMaterial(materialFila3);

    // FILA 4 

    // otra bola
    glm::vec3 centre4 = glm::vec3(5.0f, 0.0f, -25.0f);
    float radio4 = 3.0f; 
    glm::vec3 color4 = glm::vec3(0.65f, 0.77f, 0.97f);

    objects.push_back(std::make_shared<Sphere>(centre4, radio4, color4)); // Esfera azul

    auto materialFila4 = std::make_shared<Lambertian>(color4);

    materialFila4->Ka = glm::vec3(0.2f, 0.2f, 0.2f);
    materialFila4->Kd = glm::vec3(0.65f, 0.77f, 0.97f);
    materialFila4->Ks = glm::vec3(1.0f, 1.0f, 1.0f);

    materialFila4->Kt = vec3(1.0f, 1.0f, 1.0f); // componente transparente
    materialFila4->opacity = 0.0f; // LO CONTRARIO A KT

    materialFila4->shininess = 10.0f; // BETA
    materialFila4->ior = 1.0f; 

    objects[3]->setMaterial(materialFila4);

    // FILA 5 

    // bola gris
    glm::vec3 centre5 = glm::vec3(-5.5f, 0.0f, -15.0f);
    float radio5 = 3.0f; 
    glm::vec3 color5 = glm::vec3(0.90f, 0.90f, 0.90f);

    objects.push_back(std::make_shared<Sphere>(centre5, radio5, color5)); // Esfera gris

    auto materialFila5 = std::make_shared<Metal>(color5);

    materialFila5->Ka = glm::vec3(0.2f, 0.2f, 0.2f);
    materialFila5->Kd = glm::vec3(0.90f, 0.90f, 0.90f);
    materialFila5->Ks = glm::vec3(0.7f, 0.7f, 0.7f);

    materialFila5->Kt = vec3(1.0f, 1.0f, 1.0f); // componente transparente
    materialFila5->opacity = 0.0f; // LO CONTRARIO A KT

    materialFila5->shininess = 10.0f; // BETA
    materialFila5->ior = 1.0f; 

    objects[4]->setMaterial(materialFila5);





    computeBoundingBox();

}


void Scene::Fitxa3Test3(){

    Fitxa3Test2(); // son iguales

}

void Scene::Fitxa3Test4(){

    objects.clear();

    // FILA 1 

    // bola grande de abajo

    glm::vec3 centre = glm::vec3(0.0f, -10004.0f , -20.0f );
    float radio = 10000.0f; 
    glm::vec3 color = glm::vec3(0.5f, 0.5f , 0.5f );

    objects.push_back(std::make_shared<Sphere>(centre, radio, color)); // Esfera vermella

    auto materialFila1 = std::make_shared<Lambertian>(color);

    materialFila1->Ka = glm::vec3(0.2f,0.2f,0.2f);
    materialFila1->Kd = glm::vec3(0.2f,0.2f,0.2f);
    materialFila1->Ks = glm::vec3(1.0f,1.0f,1.0f);

    // OPACIDAD = 1 - KT

    materialFila1->Kt = vec3(0.0f,0.0f,0.0f); // component transparent?
    materialFila1->opacity= 1.0f; // LO CONTRARIO A KT

    materialFila1->shininess = 1.0f; // BETA
    materialFila1->ior= 1.0f; 

    objects[0]->setMaterial(materialFila1); 



    // FILA 2 

    // bola pequeña en el centro
    glm::vec3 centre2 = glm::vec3(0.0f, 0.0f, -20.0f);
    float radio2 = 4.0f; 
    glm::vec3 color2 = glm::vec3(1.0f, 0.32f, 0.36f);

    objects.push_back(std::make_shared<Sphere>(centre2, radio2, color2)); // Esfera roja

    auto materialFila2 = std::make_shared<Transp>(color2);

    materialFila2->Ka = glm::vec3(0.2f, 0.2f, 0.2f);
    materialFila2->Kd = glm::vec3(0.8f, 0.32f, 0.36f);
    materialFila2->Ks = glm::vec3(1.0f, 1.0f, 1.0f);

    materialFila2->Kt = vec3(0.75f, 0.75f, 0.75f); // componente transparente
    materialFila2->opacity = 0.25f; // LO CONTRARIO A KT

    materialFila2->shininess = 10.0f; // BETA
    materialFila2->ior = 1.5f; 

    objects[1]->setMaterial(materialFila2);

    // FILA 3 

    // bola intermedia
    glm::vec3 centre3 = glm::vec3(5.0f, -1.0f, -15.0f);
    float radio3 = 2.0f; 
    glm::vec3 color3 = glm::vec3(0.90f, 0.76f, 0.46f);

    objects.push_back(std::make_shared<Sphere>(centre3, radio3, color3)); // Esfera amarilla

    auto materialFila3 = std::make_shared<Metal>(color3);

    materialFila3->Ka = glm::vec3(0.2f, 0.2f, 0.2f);
    materialFila3->Kd = glm::vec3(0.90f, 0.76f, 0.46f);
    materialFila3->Ks = glm::vec3(0.7f, 0.7f, 0.7f);

    materialFila3->Kt = vec3(1.0f, 1.0f, 1.0f); // componente transparente
    materialFila3->opacity = 0.0f; // LO CONTRARIO A KT

    materialFila3->shininess = 10.0f; // BETA
    materialFila3->ior = 1.0f; 

    objects[2]->setMaterial(materialFila3);

    // FILA 4 

    // otra bola
    glm::vec3 centre4 = glm::vec3(5.0f, 0.0f, -25.0f);
    float radio4 = 3.0f; 
    glm::vec3 color4 = glm::vec3(0.65f, 0.77f, 0.97f);

    objects.push_back(std::make_shared<Sphere>(centre4, radio4, color4)); // Esfera azul

    auto materialFila4 = std::make_shared<Lambertian>(color4);

    materialFila4->Ka = glm::vec3(0.2f, 0.2f, 0.2f);
    materialFila4->Kd = glm::vec3(0.65f, 0.77f, 0.97f);
    materialFila4->Ks = glm::vec3(1.0f, 1.0f, 1.0f);

    materialFila4->Kt = vec3(1.0f, 1.0f, 1.0f); // componente transparente
    materialFila4->opacity = 0.0f; // LO CONTRARIO A KT

    materialFila4->shininess = 10.0f; // BETA
    materialFila4->ior = 1.0f; 

    objects[3]->setMaterial(materialFila4);

    // FILA 5 

    // bola gris
    glm::vec3 centre5 = glm::vec3(-5.5f, 0.0f, -15.0f);
    float radio5 = 3.0f; 
    glm::vec3 color5 = glm::vec3(0.90f, 0.90f, 0.90f);

    objects.push_back(std::make_shared<Sphere>(centre5, radio5, color5)); // Esfera gris

    auto materialFila5 = std::make_shared<Metal>(color5);

    materialFila5->Ka = glm::vec3(0.2f, 0.2f, 0.2f);
    materialFila5->Kd = glm::vec3(0.90f, 0.90f, 0.90f);
    materialFila5->Ks = glm::vec3(0.7f, 0.7f, 0.7f);

    materialFila5->Kt = vec3(1.0f, 1.0f, 1.0f); // componente transparente
    materialFila5->opacity = 0.0f; // LO CONTRARIO A KT

    materialFila5->shininess = 10.0f; // BETA
    materialFila5->ior = 1.0f; 

    objects[4]->setMaterial(materialFila5);





    computeBoundingBox();


}

void Scene::ESCENA(){

    objects.clear();

    glm::vec3 centre = glm::vec3(5.0f, 0.0f, -25.0f);
    float radio = 3.0f; 
    glm::vec3 color = glm::vec3(0.65f, 0.77f, 0.97f);

    AfegirEsferaTextura(centre, radio , color , "basquet.jpg",0);

    centre = glm::vec3(-5.0f, 0.0f, -25.0f);
    radio = 3.0f; 
    color = glm::vec3(0.65f, 0.77f, 0.97f);

    AfegirEsferaTextura(centre, radio , color , "basquet.jpg",1);

    glm::vec3 origen = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 tamany = glm::vec3(1.0f, 1.0f, 1.0f);
    color = glm::vec3(0.65f, 0.77f, 0.97f);

    AfegirBoxTextura(origen,tamany,color,"desert.jpg",2);

    objects.clear();

    centre = glm::vec3(-5.0f, 0.0f, -25.0f);
    radio = 3.0f; 
    color = glm::vec3(0.65f, 0.77f, 0.97f);

    AfegirEsferaMetall(centre,radio,color,0);


    
    centre = glm::vec3(-5.0f, 0.0f, -15.0f);
    radio = 3.0f; 
    color = glm::vec3(1.0f, 0.0f, 0.0f);

    AfegirEsferaTransparent(centre,radio,color,1);






    // x -- izquierda o derecha
    // y -- arriba o abajo
    // z -- cerca o lejos
    objects.clear();

    int indexArray = 0;

    for (int i = 0; i< 10 ; i++){

        centre = glm::vec3(4.0f, -3.0f, -5*i);
        radio = 2.0f;
        color = glm::vec3( 0.1f*i, 0.4f*i, 0.7f*i);

        if (i % 2 == 0){
            AfegirEsferaTransparent(centre,radio,color,indexArray);
        } else {
            AfegirEsferaMetall(centre,radio,color,indexArray);
        } 

        indexArray++;

    }

    for (int i = 0; i< 10 ; i++){

        centre = glm::vec3(-4.0f, -3.0f, -5*i);
        radio = 2.0f;
        color = glm::vec3( 0.7f*i, 0.4f*i, 0.1f*i);

        if (i % 2 != 0){
            AfegirEsferaTransparent(centre,radio,color,indexArray);
        } else {
            AfegirEsferaMetall(centre,radio,color,indexArray);
        } 

        indexArray++;

    }

    centre = glm::vec3(-4.0f, -3.0f, -700.0f);
    radio = 150.0f;
    color = glm::vec3( 0.7f, 0.4, 0.7f);

    AfegirEsferaTextura(centre,radio,color,"basquet.jpg",indexArray);
    indexArray++;

    objects.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, -100.5f, -5.0f), 95.0f, glm::vec3(0.8f, 0.8f, 0.0f))); // Esfera gran



    computeBoundingBox();

}


void Scene::ESCENA_BILLAR() {
    objects.clear();
    glm::vec3 color = glm::vec3(0.65f, 0.77f, 0.97f);

    // 1. Crear la "mesa" con textura de mesa de billar
    glm::vec3 origen = glm::vec3(-2.0f, -1.0f, -2.0f);
    glm::vec3 tamany = glm::vec3(6.0f, -0.5f, 6.0f);
    color = glm::vec3(0.65f, 0.77f, 0.97f);

    AfegirBoxTextura(origen, tamany, color, "mesa_billar.jpg", 0);

    // 2. Posicionar el triángulo centrado en la mesa pero más cerca del borde superior
    glm::vec3 centroTriangulo = glm::vec3(1.75f, -0.125f, 4.5f); // Más centrado en X y cerca del borde superior
    float radioBola = 0.25f;

    // Triángulo de bolas
    AfegirEsferaTextura(centroTriangulo + glm::vec3(0.0f, 0.0f, -1.0f), radioBola, glm::vec3(1.0f, 1.0f, 1.0f), "pintura.jpg", 1);
   // AfegirEsferaMetall(centroTriangulo + glm::vec3(0.0f, 0.0f, -1.0f), radioBola, glm::vec3(1.0f, 3.0f, 5.0f), 1);
    AfegirEsferaTransparent(centroTriangulo + glm::vec3(-0.25f, 0.0f, -0.75f), radioBola, glm::vec3(9.0f, 7.0f, 8.0f), 2);
    //AfegirEsferaMetall(centroTriangulo + glm::vec3(0.25f, 0.0f, -0.75f), radioBola, glm::vec3(8.0f, 4.0f, 6.0f), 3);
    AfegirEsferaTextura(centroTriangulo + glm::vec3(0.25f, 0.0f, -0.75f), radioBola, glm::vec3(1.0f, 1.0f, 1.0f), "desert.jpg", 3);
    AfegirEsferaTransparent(centroTriangulo + glm::vec3(-0.5f, 0.0f, -0.5f), radioBola, glm::vec3(1.0f, 7.0f, 2.0f), 4);
    AfegirEsferaTextura(centroTriangulo + glm::vec3(0.0f, 0.0f, -0.5f), radioBola, glm::vec3(1.0f, 1.0f, 1.0f), "bola_8.png", 5);
    //AfegirEsferaMetall(centroTriangulo + glm::vec3(0.0f, 0.0f, -0.5f), radioBola, glm::vec3(1.0f, 8.0f, 6.0f), 5);
    AfegirEsferaTransparent(centroTriangulo + glm::vec3(0.5f, 0.0f, -0.5f), radioBola, glm::vec3(9.0f, 9.0f, 1.0f), 6);
    AfegirEsferaMetall(centroTriangulo + glm::vec3(-0.75f, 0.0f, -0.25f), radioBola, glm::vec3(1.0f, 1.0f, 4.0f), 7);
    //AfegirEsferaTransparent(centroTriangulo + glm::vec3(-0.25f, 0.0f, -0.25f), radioBola, glm::vec3(5.0f, 7.0f, 2.0f), 8);
    AfegirEsferaTextura(centroTriangulo + glm::vec3(-0.25f, 0.0f, -0.25f), radioBola, glm::vec3(1.0f, 1.0f, 1.0f), "tenis.jpg", 8);
    AfegirEsferaMetall(centroTriangulo + glm::vec3(0.25f, 0.0f, -0.25f), radioBola, glm::vec3(1.0f, 0.0f, 0.0f), 9);
    AfegirEsferaTransparent(centroTriangulo + glm::vec3(0.75f, 0.0f, -0.25f), radioBola, glm::vec3(6.0f, 5.0f, 3.0f), 10);
    //AfegirEsferaMetall(centroTriangulo + glm::vec3(-1.0f, 0.0f, 0.0f), radioBola, glm::vec3(0.0f, 6.0f, 1.0f), 11);
    AfegirEsferaTextura(centroTriangulo + glm::vec3(-1.0f, 0.0f, 0.0f), radioBola, glm::vec3(1.0f, 1.0f, 1.0f), "volley.jpg", 11);
    AfegirEsferaTransparent(centroTriangulo + glm::vec3(-0.5f, 0.0f, 0.0f), radioBola, glm::vec3(7.0f, 9.0f, 1.0f), 12);
    AfegirEsferaMetall(centroTriangulo + glm::vec3(0.0f, 0.0f, 0.0f), radioBola, glm::vec3(1.0f, 1.0f, 7.0f), 13);
    AfegirEsferaTransparent(centroTriangulo + glm::vec3(0.5f, 0.0f, 0.0f), radioBola, glm::vec3(1.0f, 4.0f, 1.0f), 14);
    //AfegirEsferaMetall(centroTriangulo + glm::vec3(1.0f, 0.0f, 0.0f), radioBola, glm::vec3(1.0f, 1.0f, 0.0f), 15);
    AfegirEsferaTextura(centroTriangulo + glm::vec3(1.0f, 0.0f, 0.0f), radioBola, glm::vec3(1.0f, 1.0f, 1.0f), "basquet.jpg", 15);

    // 3. Colocar la bola blanca más centrada y en la parte inferior
    glm::vec3 posicionBolaBlanca = glm::vec3(1.75f, -0.125f, 1.25f); // Más centrada en X y a una buena distancia
    AfegirEsferaMetall(posicionBolaBlanca, radioBola, glm::vec3(1.0f, 1.0f, 1.0f), 16);
    //AfegirEsferaMetall(posicionBolaBlanca, radioBola, glm::vec3(1.0f, 1.0f, 1.0f), 16);

    //glm::vec3 pos = glm::vec3(1.75f, -0.125f, -4.0f); // Más centrada en X y a una buena distancia
    //AfegirEsferaTextura(pos, radioBola, glm::vec3(1.0f, 1.0f, 1.0f), "bola_blanca.jpg", 17);

    std::filesystem::path base_path_bola = std::filesystem::current_path();
    base_path_bola = base_path_bola.parent_path();  // Retrocede un nivel en la jerarquía de directorios (estamos en build)



    // Combina la ruta base con la ruta de la textura
    std::filesystem::path texture_path_bola = base_path_bola / "imagenesMemoria" / "imgEscenas" / "obj" / "homer.obj";
    auto object = std::make_shared<Mesh>(texture_path_bola.string().c_str());
    object->translate(vec3(1.75f, -0.125f, -4.0f));
    objects.push_back(object);

    auto material = std::make_shared<Lambertian>(glm::vec3(1.0f, 0.0f, 0.0f));

    material->Kd = vec3(1.0f,1.0f,0.0f);

    objects[17]->setMaterial(material);


    computeBoundingBox();
}


void Scene::AfegirEsferaTextura( vec3 centre , float radi , vec3 color , string archiu , int index ){


     // esfera

     
    //objects.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, 0.0f), radio, glm::vec3(0.3f, 0.0f, 0.2f))); // Esfera vermella
    objects.push_back(std::make_shared<Sphere>(centre, radi, color)); // Esfera vermella

    // Obtiene la ruta base del proyecto
    std::filesystem::path base_path_bola = std::filesystem::current_path();
    base_path_bola = base_path_bola.parent_path();  // Retrocede un nivel en la jerarquía de directorios (estamos en build)



    // Combina la ruta base con la ruta de la textura
    std::filesystem::path texture_path_bola = base_path_bola / "imagenesMemoria" / "imgEscenas" / archiu;

    //std::string absolute_texture_path_bola = "C:\\Users\\rioan\\OneDrive\\Documents\\3eroooo de UNI\\Segundo Cuatri\\Grafics\\Practica 1\\Capturas\\emojiPng.png";
    
    /*
    std::string absolute_texture_path_bola = "C:\\Users\\rioan\\OneDrive\\Documents\\3eroooo de UNI\\Segundo Cuatri\\Grafics\\Practica 1\\p1-pathtracingtoy-b08\\resources\\desert.jpg";

    if (!std::filesystem::exists(absolute_texture_path_bola)) {
        std::cerr << "El archivo no existe: " << absolute_texture_path_bola << std::endl;
    } else {
        std::cout << "Archivo encontrado: " << absolute_texture_path_bola << std::endl;
    }
    */

    if (!std::filesystem::exists(texture_path_bola)) {
        std::cerr << "El archivo no existe: " << texture_path_bola << std::endl;
    } else {
        std::cout << "Archivo encontrado: " << texture_path_bola << std::endl;
    }
        

    // Crea un material de textura con el color base
    auto materialTexturaBola = std::make_shared<MaterialTextura>();

    // Crea un objeto Image utilizando la ruta de la textura y asegúrate de que sea el tipo correcto
    //materialTexturaBola->background = std::make_shared<Image>(absolute_texture_path_bola.c_str()); 
    materialTexturaBola->background = std::make_shared<Image>(texture_path_bola.string().c_str()); 

    // Asigna el material a la caja 
    objects[index]->setMaterial(materialTexturaBola); 

}


void Scene::AfegirBoxTextura(vec3 origen, vec3 tamany , vec3 color , string archiu, int index) {

    // BOX AMB TEXTURE

    // Crea una caja en el origen (0, 0, 0) con tamaño (1, 1, 1) y un color base
    objects.push_back(std::make_shared<Box>(origen, tamany, color )); 

    glm::vec3 color1 = glm::vec3(0.8f, 0.8f, 0.8f);

    // Obtiene la ruta base del proyecto
    std::filesystem::path base_path = std::filesystem::current_path();
    base_path = base_path.parent_path();  // Retrocede un nivel en la jerarquía de directorios (estamos en build)

    // Combina la ruta base con la ruta de la textura
    std::filesystem::path texture_path = base_path / "imagenesMemoria" / "imgEscenas" / archiu;

    if (!std::filesystem::exists(texture_path)) {
        std::cerr << "El archivo no existe: " << texture_path << std::endl;
    } else {
        std::cout << "Archivo encontrado: " << texture_path << std::endl;
    }

    // Crea un material de textura con el color base
    auto materialTextura = std::make_shared<MaterialTextura>(color1);

    // Crea un objeto Image utilizando la ruta de la textura y asegúrate de que sea el tipo correcto
    materialTextura->background = std::make_shared<Image>(texture_path.string().c_str()); 

    // Asigna el material a la caja 
    objects[index]->setMaterial(materialTextura); 
}

void Scene::AfegirEsferaMetall(vec3 centre , float radi , vec3 color , int index ){

    objects.push_back(std::make_shared<Sphere>(centre, radi, color )); // Esfera vermella    
    
    // la bola grande de metal
    auto material = std::make_shared<Metal>(glm::vec3(0.8f, 0.8f, 0.0f));

    material->Ka = vec3(0.2f,0.2f,0.2f); // component ambient
    material->Kd = color; // component difusa
    material->Ks = vec3(0.8f,0.8f,0.8f); // component especular

    material->shininess = 100.0f; 

    objects[index]->setMaterial(material); 

}

void Scene::AfegirEsferaTransparent(vec3 centre , float radi , vec3 color , int index ){

    objects.push_back(std::make_shared<Sphere>(centre, radi, color )); // Esfera vermella    

    auto materialTransparente = std::make_shared<Transp>(color);

    materialTransparente->Ka = vec3(0.2f,0.2f,0.2f); // component ambient
    materialTransparente->Kd = color ; // component difusa
    materialTransparente->Ks = vec3(1.0f,1.0f,1.0f); // component especular

    materialTransparente->Kt = vec3(0.9f,0.9f,0.9f); // component transparent?

    materialTransparente->shininess = 10.0f; 
    materialTransparente->opacity= 0.1f; // LO CONTRARIO A KT
    materialTransparente->ior= 1.0f; 

    objects[index]->setMaterial(materialTransparente); 

}

void Scene::testOmbres(){
    // Escenario original con todo metalico

    objects.clear(); // Limpiar la lista de objetos antes de añadir nuevos
    //objects.push_back(std::make_shared<Box>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
    
    objects.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, -5.0f), 1.0f, glm::vec3(1.0f, 0.0f, 0.0f))); 


    auto materialPrueba = std::make_shared<Metal>(glm::vec3(1.0f, 0.0f, 0.0f));


    materialPrueba->Ka = vec3(0.2f,0.2f,0.2f); // component ambient
    materialPrueba->Kd = vec3(0.8f,0.8f,0.0f); // component difusa
    materialPrueba->Ks = vec3(1.0f,1.0f,1.0f); // component especular

    materialPrueba->shininess = 100.0f;

    objects.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, -100.5f, -5.0f), 100.0f, glm::vec3(0.8f, 0.8f, 0.0f))); 
    objects.push_back(std::make_shared<Sphere>(glm::vec3(2.0f, 1.0f, -6.0f), 1.5f, glm::vec3(0.0f, 1.0f, 0.0f))); 
    objects.push_back(std::make_shared<Sphere>(glm::vec3(-2.0f, 1.0f, -4.0f), 1.0f, glm::vec3(0.0f, 0.0f, 1.0f))); 
    

    /*
    for (auto& obj : objects) {
        obj->setMaterial(std::make_shared<Metal>(obj->getMaterial()->albedo));
        //obj->setMaterial(materialPrueba);
    }
    */
    // Esfera transparente

    // Crear un material transparente
    glm::vec3 colorTransparente = glm::vec3(0.0f, 0.5f, 1.0f); // Azul claro
    auto materialTranspa = std::make_shared<Transp>(colorTransparente);

    materialTranspa->Ka = glm::vec3(0.1f, 0.1f, 0.1f);  // Ambiente
    materialTranspa->Kd = glm::vec3(0.2f, 0.2f, 0.2f);  // Difusa
    materialTranspa->Ks = glm::vec3(1.0f, 1.0f, 1.0f);  // Especular
    materialTranspa->Kt = glm::vec3(0.7f, 0.7f, 0.7f); // Brillo
    materialTranspa->opacity = 0.3f; // Más transparente
    materialTranspa->ior = 1.0f; // Índice de refracción de vidrio

    // Actualizar dmax usando setDmax()
    float nuevoDmax = std::max(materialTranspa->getDmax(), glm::length(glm::vec3(10.0f, 10.0f, 10.0f)));
    materialTranspa->setDmax(nuevoDmax);

    objects[3]->setMaterial(materialTranspa);

    computeBoundingBox();
}

void Scene::pathtracing(){

    objects.clear();

    objects.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, -5.0f), 1.0f, glm::vec3(1.0f, 0.0f, 0.0f)));  // roja
    
    auto material = std::make_shared<Lambertian>(glm::vec3(1.0f, 0.0f, 0.0f));

    //material->Kt = glm::vec3(0.7f, 0.7f, 0.7f); // Brillo
    //material->opacity = 0.3f; // Más transparente
    //material->ior = 1.0f; // Índice de refracción de vidrio
    
    material->roughness = 1 ;

    objects[0]->setMaterial(material);


    
    objects.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, -100.5f, -5.0f), 100.0f, glm::vec3(0.8f, 0.8f, 0.0f))); // amarilla grande

    material = std::make_shared<Lambertian>(glm::vec3(0.8f, 0.8f, 0.0f));
    material->roughness = 1 ;

    objects[1]->setMaterial(material);

    computeBoundingBox();

}

void Scene::mesh(){

    objects.clear();

    //std::string absolute_texture_path = "C:\\Users\\rioan\\OneDrive\\Documents\\3eroooo de UNI\\Segundo Cuatri\\Grafics\\Practica 1\\p1-pathtracingtoy-b08\\imagenesMemoria\\imgEscenas\\sword.obj";
    //std::string absolute_texture_path = "C:\\Users\\rioan\\OneDrive\\Documents\\3eroooo de UNI\\Segundo Cuatri\\Grafics\\Practica 1\\p1-pathtracingtoy-b08\\imagenesMemoria\\imgEscenas\\tripleM.obj";
    //std::string absolute_texture_path = "C:\\Users\\rioan\\OneDrive\\Documents\\3eroooo de UNI\\Segundo Cuatri\\Grafics\\Practica 1\\p1-pathtracingtoy-b08\\imagenesMemoria\\imgEscenas\\obj\\city_stall.obj";
    //std::string absolute_texture_path = "C:\\Users\\rioan\\OneDrive\\Documents\\3eroooo de UNI\\Segundo Cuatri\\Grafics\\Practica 1\\p1-pathtracingtoy-b08\\imagenesMemoria\\imgEscenas\\cat.obj";
    //std::string absolute_texture_path = "C:\\Users\\rioan\\OneDrive\\Documents\\3eroooo de UNI\\Segundo Cuatri\\Grafics\\Practica 1\\p1-pathtracingtoy-b08\\imagenesMemoria\\imgEscenas\\samsung.obj";
    //std::string absolute_texture_path = "C:\\Users\\rioan\\OneDrive\\Documents\\3eroooo de UNI\\Segundo Cuatri\\Grafics\\Practica 1\\p1-pathtracingtoy-b08\\imagenesMemoria\\imgEscenas\\airplane.obj";
    //std::string absolute_texture_path = "C:\\Users\\rioan\\OneDrive\\Documents\\3eroooo de UNI\\Segundo Cuatri\\Grafics\\Practica 1\\p1-pathtracingtoy-b08\\imagenesMemoria\\imgEscenas\\obj\\house.obj";


    //std::string absolute_texture_path = "C:\\Users\\rioan\\OneDrive\\Documents\\3eroooo de UNI\\Segundo Cuatri\\Grafics\\Practica 1\\p1-pathtracingtoy-b08\\imagenesMemoria\\imgEscenas\\obj\\city_stall.obj"; // poner factor a 0.01
    //std::string absolute_texture_path = "C:\\Users\\rioan\\OneDrive\\Documents\\3eroooo de UNI\\Segundo Cuatri\\Grafics\\Practica 1\\p1-pathtracingtoy-b08\\imagenesMemoria\\imgEscenas\\obj\\house2.obj";
    //std::string absolute_texture_path = "C:\\Users\\rioan\\OneDrive\\Documents\\3eroooo de UNI\\Segundo Cuatri\\Grafics\\Practica 1\\p1-pathtracingtoy-b08\\imagenesMemoria\\imgEscenas\\obj\\lamp2.obj";
    //std::string absolute_texture_path = "C:\\Users\\rioan\\OneDrive\\Documents\\3eroooo de UNI\\Segundo Cuatri\\Grafics\\Practica 1\\p1-pathtracingtoy-b08\\imagenesMemoria\\imgEscenas\\obj\\lamp3.obj";

    std::filesystem::path base_path_bola = std::filesystem::current_path();
    base_path_bola = base_path_bola.parent_path();  // Retrocede un nivel en la jerarquía de directorios (estamos en build)



    // Combina la ruta base con la ruta de la textura
    std::filesystem::path texture_path_bola = base_path_bola / "imagenesMemoria" / "imgEscenas" / "obj" / "city_stall.obj";

    //std::string absolute_texture_path = "C:\\Users\\rioan\\OneDrive\\Documents\\3eroooo de UNI\\Segundo Cuatri\\Grafics\\Practica 1\\p1-pathtracingtoy-b08\\imagenesMemoria\\imgEscenas\\obj\\city_stall.obj"; // poner factor a 0.01
    //std::string absolute_texture_path = "C:\\Users\\rioan\\OneDrive\\Documents\\3eroooo de UNI\\Segundo Cuatri\\Grafics\\Practica 1\\p1-pathtracingtoy-b08\\imagenesMemoria\\imgEscenas\\obj\\house2.obj";
    //std::string absolute_texture_path = "C:\\Users\\rioan\\OneDrive\\Documents\\3eroooo de UNI\\Segundo Cuatri\\Grafics\\Practica 1\\p1-pathtracingtoy-b08\\imagenesMemoria\\imgEscenas\\obj\\lamp2.obj";
    //std::string absolute_texture_path = "C:\\Users\\rioan\\OneDrive\\Documents\\3eroooo de UNI\\Segundo Cuatri\\Grafics\\Practica 1\\p1-pathtracingtoy-b08\\imagenesMemoria\\imgEscenas\\obj\\lamp3.obj";

    auto object = std::make_shared<Mesh>(texture_path_bola.string().c_str());

    objects.push_back(object);

    auto material = std::make_shared<Lambertian>(glm::vec3(1.0f, 0.0f, 0.0f));

    material->Kd = vec3(1.0f,0.0f,0.0f);

    objects[0]->setMaterial(material);

    computeBoundingBox();

}