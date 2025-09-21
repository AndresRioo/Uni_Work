#include "World.hpp"

World::World() {
    camera = make_shared<Camera>();
    scene = make_shared<Scene>();
}

void World::renderWorld(Image &framebuffer, shared_ptr<Config> config) {
    
    setConfig(config);

    // Seleccionamos la estrategia de sombreado según el valor en Config
    shared_ptr<ShadingStrategy> shadingStrategy;

    scene->useBoundingBoxOptimization = config->boundingBox;

    switch (config->selectedShader) {
        case 0  :  // NormalShading
            shadingStrategy = make_shared<NormalShading>(scene, lights, config->observador,  config->shadow);
            break;
        case 1 :  // DiffuseShading 
            shadingStrategy = make_shared<DiffuseShading>(scene, lights, config->observador , config->shadow);
            break;
        case 2:  // BlinnPhongShading
            shadingStrategy = make_shared<BlinnPhongShading>(scene, lights, config->observador , config->shadow);
            break;
        case 3:  // ToonShading
            shadingStrategy = make_shared<ToonShading>(scene, lights, config->observador , config->shadow);
            break;
        default:
            shadingStrategy = make_shared<NormalShading>(scene, lights, config->observador, false); // Por defecto
            break;
    }

    // TODO: Creació d'un tracer, segons la configuració
    // Per ara es crea un Raycasting
    shared_ptr<Render> render;

    switch ( config->selectedTracer ) {
        case 0 : // Raycasting
            render = make_shared<Raycasting>(scene, lights, camera, config, shadingStrategy);
            break;
        case 1 : // RayTracing
            render = make_shared<Raytracing>(scene, lights, camera, config, shadingStrategy);
            break;
        case 2 : 
            render = make_shared<Pathtracing>(scene, lights, camera, config, shadingStrategy);
            break;
        case 3 : 
            render = make_shared<examen>(scene, lights, camera, config, shadingStrategy);
            break;
        default:
            render = make_shared<Raycasting>(scene, lights, camera, config, shadingStrategy);
    }
    
    float width = setup->viewportWidth;
    float height = setup->viewportHeight;
    float scale = tan(glm::radians(config->fov) / 2.0f);
    framebuffer.resize(width, height);

    float aspectRatio = static_cast<float>(width) / height;
    
    // Calcula el vector de direcció des de la càmera al VRP i la resta d'eixos de la càmera
    glm::vec3 cameraDirection = glm::normalize(config->observador-config->vrp);
    glm::vec3 cameraRight = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), cameraDirection));
    glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);


    for (int y = 0; y < height; y++) {
        std::cout << "\r [ "<< (int)(100*(y+1)/height)<< "% ] Rendering in progress "<<std::flush;

        for (int x = 0; x < width; x++) {

            glm::vec3 pixelColor(0.0f);

            // Llançar múltiples rajos per a cada píxel
            for (int i = 0; i < config->numSamples; i++) {
                // Petita variació dins del píxel per cada mostra

                // rand genera un valor entre 0 i randMAX
                // dividim entre rand max per obtenir [0,1]
                // restem -0.5f per tenir valors d'offset de [-0.5f,0.5f]
                // dividim entre width y height per assegurar que el desplaçament sigui fraccional dins del píxel, no fora d’ell.
                float uOffset = (rand() / (float)RAND_MAX - 0.5f);// / width;
                float vOffset = (rand() / (float)RAND_MAX - 0.5f);// / height;

                if (config->numSamples == 1){
                    uOffset = 0;
                    vOffset = 0; // evitar distorsions al fer cambis a altres parametres 
                }

                float u = (2.0f * (x + uOffset)) / width - 1.0f;
                float v = 1.0f - 2.0f * (y + vOffset) / height;

                // Ajustar aspect ratio i FOV
                u = u * scale * aspectRatio;
                v = v * scale;

                // Generar direcció del raig
                glm::vec3 rayDirection = glm::normalize(-cameraDirection + u * cameraRight + v * cameraUp);
                Ray ray(config->observador, rayDirection);

                // Obtenir color del raig traçat
                pixelColor += render->tracer(ray);
            }

            // Fer la mitjana dels colors
            pixelColor /= (float)config->numSamples;

            // Clamp dels valors de color entre 0 i 1
            pixelColor = glm::clamp(pixelColor, 0.0f, 1.0f);

            // Escriure el color final al framebuffer
            framebuffer.setPixelColor(x, y, pixelColor);






            /*
            // Ajusta les coordenades normalitzades de la pantalla
            float u = (2.0f * x) / width - 1.0f;  
            float v = 1.0f - 2.0f * y / height;

            // Ajusta l'aspect ratio en la coordenada X, suposant que width>height
            // Aplica l'escala del fov
            u = u * scale * aspectRatio;
            v = v * scale;

            // Genera la direcció del raig basant-se en la càmera
            glm::vec3 rayDirection = glm::normalize(-cameraDirection + u * cameraRight + v * cameraUp);
            Ray ray(config->observador, rayDirection);

            // Crida al render per obtenir el color del pixel
            glm::vec3 pixelColor(0.0f);
            pixelColor = render->tracer(ray);

            pixelColor = glm::clamp(pixelColor,0.0f,1.0f);

            // Escriu el color al framebuffer
            framebuffer.setPixelColor(x, y, pixelColor);

            */
    
        }
    }
}
void World::changeCamera(glm::vec3 obs, glm::vec3 vrp, glm::vec3 up, float fov) {
    camera->position = obs;
    camera->vrp = vrp;
    camera->up = up;
    camera->fov = fov;
}

void World::generateRandomSpheres(int numSpheres)
{
    scene->generateRandomSpheres(numSpheres);
}

void World::refreshProperties() 
{
    camera->position = setup->observador;
    camera->vrp = setup->vrp;

    if (lights.size() == 0) {
        auto light = make_shared<PointLight>(setup->lightPos, setup->lightAmbient, setup->lightDiffuse, setup->lightSpecular);
        light->setIaGlobal(setup->lightAmbientGlobal);
        lights.push_back(light);
    }
    else {
        lights[0]->setIa(setup->lightAmbient);
        lights[0]->setId(setup->lightDiffuse);
        lights[0]->setIs(setup->lightSpecular);
        lights[0]->setIaGlobal(setup->lightAmbientGlobal);

        if (lights[0] == dynamic_pointer_cast<PointLight>(lights[0])) {
            dynamic_pointer_cast<PointLight>(lights[0])->setPos(setup->lightPos);
        }
    } 
}

void World::generateAnimation(int numFrames)
{
    scene->generateProceduralAnimation(numFrames);
}

void World::update(int nframe)
{
    scene->update(nframe);
}