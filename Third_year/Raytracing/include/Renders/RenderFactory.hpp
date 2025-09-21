#pragma once

#include "Renders/Render.hpp"
#include "Renders/Raycasting.hpp"
#include "Renders/Raytracing.hpp"

class RenderFactory {
    RenderFactory(){};
public:
    typedef enum  {
        RAYCASTING,
        RAYTRACING,
        PATHTRACING
    } RENDER_TYPES;

    static RenderFactory& getInstance() {
        static RenderFactory instance;
        return instance;
    }
    
    shared_ptr<Render> createRender( RENDER_TYPES t);
    RENDER_TYPES getIndexType (shared_ptr<Render> r);
    shared_ptr<Render> createRender( RENDER_TYPES t);
};
