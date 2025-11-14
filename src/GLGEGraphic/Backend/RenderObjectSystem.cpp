/**
 * @file RenderObjectSystem.cpp
 * @author DM8AT
 * @brief implement the functions for the render object system
 * @version 0.1
 * @date 2025-11-10
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//add the render object system
#include "RenderObjectSystem.h"

RenderObjectHandle GLGE::Graphic::Backend::RenderObjectSystem::create() noexcept {
    //thread safety
    std::unique_lock lock(m_mutex);

    //check if the free list has any entries
    if (m_freeList.size()) {
        //the free list has entries
        //just use the first free entry
        uint32_t id = m_freeList.back();
        m_freeList.pop_back();
        //create the handle
        return (m_handles[id] & GLGE_RENDER_OBJECT_HANDLE_VERSION) | (id & GLGE_RENDER_OBJECT_HANDLE_INDEX);
    } else {
        //there are no free entries left
        //create a new entry
        RenderObjectHandle handle = (1u << GLGE_RENDER_OBJECT_HANDLE_VERSION_OFFSET) | ((uint32_t)m_handles.size() & GLGE_RENDER_OBJECT_HANDLE_INDEX);
        m_handles.push_back(handle);
        //return the handle
        return handle;
    }
}

void GLGE::Graphic::Backend::RenderObjectSystem::destroy(RenderObjectHandle handle) noexcept {
    //thread safety
    std::unique_lock lock(m_mutex);

    //sanity check the handle
    if ((handle & GLGE_RENDER_OBJECT_HANDLE_INDEX) >= m_handles.size()) {return;}
    if ((handle & GLGE_RENDER_OBJECT_HANDLE_VERSION) != (m_handles[handle & GLGE_RENDER_OBJECT_HANDLE_INDEX] & GLGE_RENDER_OBJECT_HANDLE_VERSION))
        {return;}

    //increase the version
    m_handles[handle & GLGE_RENDER_OBJECT_HANDLE_INDEX] += 1u << GLGE_RENDER_OBJECT_HANDLE_VERSION_OFFSET;
    //add the index to the free list
    m_freeList.push_back(handle & GLGE_RENDER_OBJECT_HANDLE_INDEX);
}