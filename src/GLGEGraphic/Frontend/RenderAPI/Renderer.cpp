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
#include "../../Backend/RenderObjectSystem.h"

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