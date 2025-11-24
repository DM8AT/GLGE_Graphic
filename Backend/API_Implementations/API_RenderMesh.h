/**
 * @file API_RenderMesh.h
 * @author DM8AT
 * @brief define the unified API for render meshes
 * @version 0.1
 * @date 2025-11-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//header guard
#ifndef _GLGE_GRAPHIC_BACKEND_API_IMPL_RENDER_MESH_
#define _GLGE_GRAPHIC_BACKEND_API_IMPL_RENDER_MESH_

//only available for C++
#if __cplusplus

//define the frontend render mesh
class RenderMesh;

//add instances
#include "API_Instance.h"

//use the GLGE::Graphic::Backend::API namespace
namespace GLGE::Graphic::Backend::API
{

/**
 * @brief define the API for render meshes
 * 
 * a render mesh is just a wrapper around a core mesh so it can be rendered
 */
class RenderMesh
{
public:

    /**
     * @brief Construct a new Render Mesh
     * 
     * @param rMesh a pointer to the frontend render mesh to create from
     */
    RenderMesh(::RenderMesh* rMesh);

    /**
     * @brief Destroy the Render Mesh
     */
    ~RenderMesh();

    /**
     * @brief Get the Vertex Pointer
     * 
     * @return const MemoryArena::GraphicPointer& the pointer at which the vertex data lives in the vertex buffer
     */
    inline const MemoryArena::GraphicPointer& getVertexPointer() const noexcept {return m_vboPointer;}

    /**
     * @brief Get the Index Pointer
     * 
     * @return const MemoryArena::GraphicPointer& the pointer at which the index data lives in the index buffer
     */
    inline const MemoryArena::GraphicPointer& getIndexPointer() const noexcept {return m_iboPointer;}

    /**
     * @brief Get the Render Mesh that belongs to the render mesh API
     * 
     * @return ::RenderMesh* a pointer to the frontend render mesh
     */
    inline ::RenderMesh* getRenderMesh() const noexcept {return m_rMesh;}

    /**
     * @brief get the GPU data of the render mesh
     * 
     * @return const GPU& a constant reference to the GPU data
     */
    inline const MeshGPUInfo& getGPUData() const noexcept {return m_gpu;}

protected:

    //store a pointer to the frontend render mesh
    ::RenderMesh* m_rMesh = nullptr;
    //store a graphic pointer to the vertex data in the memory arena
    MemoryArena::GraphicPointer m_vboPointer;
    //store a graphic pointer to the index data in the memory arena
    MemoryArena::GraphicPointer m_iboPointer;
    //store the GPU data
    MeshGPUInfo m_gpu;

};

}

#endif

#endif