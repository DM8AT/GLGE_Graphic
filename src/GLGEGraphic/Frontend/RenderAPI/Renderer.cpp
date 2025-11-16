/**
 * @file Renderer.cpp
 * @author DM8AT
 * @brief implement the renderer
 * @version 0.1
 * @date 2025-11-06
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//add the renderer
#include "Renderer.h"
//add the render object system
#include "../../Backend/Objects/RenderObjectSystem.h"
//add scenes
#include "../../../GLGE_Core/Geometry/Structure/ECS/Scene.h"

Renderer::Renderer(const RenderObject* objs, size_t objCount) noexcept 
{
    //make enough space for the objects
    m_objs.resize(objCount);
    //iterate over all objects and just copy them over
    memcpy(m_objs.data(), objs, objCount * sizeof(RenderObject));
    //create the handle
    m_handle = GLGE::Graphic::Backend::RenderObjectSystem::create();
}

Renderer::~Renderer() noexcept {
    //if the handle exists, delete it
    if (m_handle) {
        GLGE::Graphic::Backend::RenderObjectSystem::destroy(m_handle);
        m_handle = 0;
    }
}

void Renderer::setObject(Object obj)
{
    //if the object is not null, stop
    if (m_obj) {return;}
    //else, store the object
    m_obj = obj;
    //store the transform
    GLGE::Graphic::Backend::RenderObjectSystem::getTransformBuffer()->
        set(m_handle & GLGE_RENDER_OBJECT_HANDLE_INDEX, GLGE::Graphic::Backend::CompressedTransform(
            *m_obj->get<Transform>(), 
            m_handle & GLGE_RENDER_OBJECT_HANDLE_VERSION >> GLGE_RENDER_OBJECT_HANDLE_VERSION_OFFSET)
        );
}

void Renderer::reupload() noexcept {
    //if the object is null, stop
    if (!m_obj) {return;}

    //re-upload the data at the correct place
    GLGE::Graphic::Backend::RenderObjectSystem::getTransformBuffer()->
        set(m_handle & GLGE_RENDER_OBJECT_HANDLE_INDEX, GLGE::Graphic::Backend::CompressedTransform(
            *m_obj->get<Transform>(), 
            m_handle & GLGE_RENDER_OBJECT_HANDLE_VERSION >> GLGE_RENDER_OBJECT_HANDLE_VERSION_OFFSET)
        );
}