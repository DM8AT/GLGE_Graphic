/**
 * @file RenderMeshRegistry.h
 * @author DM8AT
 * @brief define a global collection for all render meshes
 * @version 0.1
 * @date 2025-11-06
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//header guard
#ifndef _GLGE_GRAPHIC_FRONTEND_RENDER_API_RENDER_MESH_REGISTRY_
#define _GLGE_GRAPHIC_FRONTEND_RENDER_API_RENDER_MESH_REGISTRY_

//add the types
#include "../../../GLGE_Core/Types.h"
//add render meshes
#include "RenderMesh.h"

/**
 * @brief store a handle to the render mesh
 * 
 * This stores only uint32_t's for the size because then the size of the render mesh handles
 * is 8 byte (64 bit) which fits into the word size of most modern computers (64-bit systems)
 */
typedef struct s_RenderMeshHandle {
    //store the index into the registry the render mesh lives at
    uint32_t idx;
    //store the version of the mesh
    uint32_t version;

    //add some utility functions for C++
    #if __cplusplus

    /**
     * @brief access the mesh of the handle
     * 
     * @return RenderMesh* a pointer to the mesh handle
     */
    inline RenderMesh* operator->(void) noexcept;

    #endif

} RenderMeshHandle;

//the class is only available for C++
#if __cplusplus

//add deque so the insertion and deletion does not invalidate existing pointers
//because this is from the std library, OS-Dependent optimizations MAY be applied
#include <deque>
//add atomics to make versions thread safe
#include <atomic>
//vectors are needed for the free list
#include <vector>
//a mutex is required to make the vector thread safe
#include <mutex>

/**
 * @brief a class that is responsible to store and manage render meshes
 * 
 * This is done so the storage location of render meshes is mostly constant. This makes access a lot easier. 
 * 
 * This is a class because a namespace could not use private static members (in C++ 23). 
 */
class RenderMeshRegistry
{
public:

    /**
     * @brief create a new render mesh
     * 
     * @param mesh the actual mesh to create the render mesh for
     * @return RenderMeshHandle the handle for the render mesh
     */
    static RenderMeshHandle create(Mesh* mesh) noexcept;

    /**
     * @brief delete the render mesh stored at the specific handle
     * 
     * @param handle the handle to destroy
     */
    static void destroy(RenderMeshHandle handle) noexcept;

    /**
     * @brief check if a specific handle is valid
     * 
     * @param handle the handle to check
     * @return true : the handle is valid
     * @return false : the handle is not valid
     */
    inline static bool isValid(RenderMeshHandle handle) noexcept 
    {return (handle.idx < m_meshes.size()) ? (handle.version == m_versions[handle.idx].load(std::memory_order_acquire)) : false;}

    /**
     * @brief access a render mesh by it's handle
     * 
     * @param handle the handle to quarry the render mesh for
     * @return RenderMesh* the render mesh or NULL if the render mesh does no longer exist
     */
    inline static RenderMesh* get(RenderMeshHandle handle) noexcept
    {return isValid(handle) ? &m_meshes[handle.idx] : nullptr;}

protected:

    //add the render mesh as a friend class
    friend class RenderMesh;

    //use std::deque to store the meshes
    //this is used so the pointers to the meshes don't get invalidated if a mesh is added or removed
    //also, iteration speed is not a big concern because the access to the meshes is allready pretty random, so data 
    //pre-fetching would not be efficient (high risk of wrong pre-fetches). 
    inline static std::deque<RenderMesh> m_meshes;
    //store the current version for each index
    //a version of the mesh. This is used to determine if a mesh is actually replaced and the handle
    //refers to an old mesh
    inline static std::deque<std::atomic_uint32_t> m_versions;

    //store all currently free indices
    //this is done to not allocate redundant memory
    inline static std::vector<uint32_t> m_freeList;
    //a mutex to protect the free list and the mesh / version deque
    inline static std::mutex m_mutex;

};

//implement the render mesh get operator here so the compiler may optimize it
RenderMesh* RenderMeshHandle::operator->(void) noexcept {return RenderMeshRegistry::get(*this);}

#endif

#endif