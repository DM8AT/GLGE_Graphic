/**
 * @file RenderMeshRegistry.cpp
 * @author DM8AT
 * @brief implement the render mesh registry
 * @version 0.1
 * @date 2025-11-07
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//add the render mesh registry
#include "RenderMeshRegistry.h"

RenderMeshHandle RenderMeshRegistry::create(Mesh* mesh) noexcept
{
    //thread safety
    std::unique_lock lock(m_mutex);

    //store the handle to return
    RenderMeshHandle handle;

    //check if a value in the free list exists
    if (m_freeList.size()) {
        //a value in the free list exists. Use that index. 
        handle.idx = m_freeList.back();
        m_freeList.pop_back();
        //get the version
        handle.version = m_versions[handle.idx].load(std::memory_order_relaxed);
        //create the new render mesh
        (void) new (&m_meshes[handle.idx]) RenderMesh(mesh, handle.idx);
    } else {
        //no free values are stored. Create a new element. 
        handle.idx = m_meshes.size();
        handle.version = 1;
        m_versions.emplace_back(handle.version);
        m_meshes.emplace_back();
        (void) new (&m_meshes[handle.idx]) RenderMesh(mesh, handle.idx);
    }

    //return the final handle
    return handle;
}

void RenderMeshRegistry::destroy(RenderMeshHandle handle) noexcept {
    //sanity check the handle
    if (!isValid(handle)) {return;}

    //thread safety
    std::unique_lock lock(m_mutex);

    //check the validity again (another thread may have deleted the mesh)
    if (!isValid(handle)) {return;}

    //update the version
    m_versions[handle.idx].fetch_add(1, std::memory_order_acq_rel);
    //clean up the object
    m_meshes[handle.idx].~RenderMesh();
    //add the index to the free list
    m_freeList.push_back(handle.idx);
}