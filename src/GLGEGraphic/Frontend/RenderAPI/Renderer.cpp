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

Renderer::Renderer(const RenderObject* objs, size_t objCount) noexcept 
{
    //make enough space for the objects
    m_objs.resize(objCount);
    //iterate over all objects and just copy them over
    memcpy(m_objs.data(), objs, objCount * sizeof(RenderObject));
}