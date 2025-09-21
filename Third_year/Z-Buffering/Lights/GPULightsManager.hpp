#pragma once

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GPULight.hpp"
#include "GPUPointLight.hpp"


class GPULightsManager  {
    private:
        // Vector de llums contingudes la mon
        vector<shared_ptr<GPUPointLight>> lights;
        // Llum ambient global:
        vec3  ambientLight;

    public:
        GPULightsManager()  {
            ambientLight = vec3(0.2f, 0.2f, 0.2f);
        }
        
        void setAmbientLight(vec3 a) {
            ambientLight = a;
        }

        void toGPU(GLuint program) {
            if (program == 0) return;
            
            // TO DO: Enviar tota la informació de la il·luminació a la GPU
            
            // TO DO: Enviar només les llums que estan actives a la GPU 
            
            // TO DO: Posar en el shader el nombre de llums actives
            

            // Enviar número de llums actives
            int numLights = 0;
            for (auto &l : lights) {
                if (l->isEnabled()) numLights++;
            }
        
            GLuint loc = glGetUniformLocation(program, "numLights");
            if (loc != -1)
                glUniform1i(loc, numLights);
                    
            // iterar cada llum y utilitzar el seu mètode implementat
            /*
            for (auto &l : lights) {
                if (!l->isEnabled()) continue;

                l->toGPU(program);
            }
            */
            int idx = 0;
            for (auto &l : lights) {
                l->toGPU(program);
                l->updateToGPU(idx);  // Siempre hay que actualizar todas las posiciones
                ++idx;
            }

        }
        

        void addLight(shared_ptr<GPUPointLight> l) {
            lights.push_back(l);
        }

        void ambientLightToGPU(GLuint program, vec3 a) {
            ambientLight = a;
            // TO DO: Enviar la llum ambient global a la GPU i revisar des d'on es crida aquest mètode

            GLuint llumAmbient; 
            llumAmbient = glGetUniformLocation(program, "ambientGlobal");
            glUniform3fv(llumAmbient, 1, glm::value_ptr(ambientLight) ); 

        }





















        void updateAllLights(GLuint program, vector<GPUPointLight> &lightsNew) {
            if (program == 0) return;
            
            lights.clear();

            for (int i = 0; i < lightsNew.size(); i++) {
                lights.push_back(make_shared<GPUPointLight>(lightsNew[i].getPos(), lightsNew[i].getIa(), lightsNew[i].getId(), lightsNew[i].getIs(), 0.0f, 0.0f, 1.0f, lightsNew[i].isEnabled()));
            
                // TO DO Fitxa 2: Cal enviar totes les llums a la GPU des d'aquest mètode?
                this->lights[i]->toGPU(program);
                this->lights[i]->updateToGPU(i);

            }            
        }

        void updateSingleLight(GLuint program, GPUPointLight &pl, int index) {
            if (program == 0) return;
            
            lights[index] = make_shared<GPUPointLight>(pl.getPos(), pl.getIa(), pl.getId(), pl.getIs(), 0.0f, 0.0f, 1.0f);
            
            // TO DO Fitxa 2: Cal només enviar la llum que està a "index" del vector de llums  a la GPU des d'aquest mètode?
            this->lights[index]->toGPU(program);
            this->lights[index]->updateToGPU(index);
            
            
        }


};