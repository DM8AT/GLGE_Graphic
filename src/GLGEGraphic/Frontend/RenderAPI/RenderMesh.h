/**
 * @file RenderMesh.h
 * @author DM8AT
 * @brief define what a render mesh is
 * @version 0.1
 * @date 2025-11-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_GRAPHIC_FRONTEND_RENDER_API_RENDER_MESH_
#define _GLGE_GRAPHIC_FRONTEND_RENDER_API_RENDER_MESH_

//for C++ define a class
#if __cplusplus

//add the core meshes
#include "../../../GLGE_Core/Geometry/Surface/Mesh.h"
//also add materials
#include "../Material.h"

//the render mesh registry will be defined later
class RenderMeshRegistry;

/**
 * @brief define the frontend API for a render mesh
 */
class RenderMesh
{
public:

    /**
     * @brief Construct a new Render Mesh
     */
    RenderMesh() = default;

    /**
     * @brief Destroy the Render Mesh
     */
    ~RenderMesh() noexcept;

    /**
     * @brief Get the core Mesh of the render mesh
     * 
     * @return Mesh* a pointer to the core mesh
     */
    inline Mesh* getMesh() const noexcept {return m_mesh;}

    //define SDL / backend stuff
    #ifdef SDL_h_

    /**
     * @brief Get the Backend object
     * 
     * @return void* 
     */
    inline void* getBackend() const noexcept {return m_backend;}

    #endif //end of the backend section

protected:

    //add the render mesh registry as a friend class
    friend class RenderMeshRegistry;

    /**
     * @brief Construct a new Render Mesh
     * 
     * @param mesh the core mesh to encapsulate
     */
    RenderMesh(Mesh* mesh) noexcept;

    //store a pointer to the core mesh
    Mesh* m_mesh = nullptr;
    //store the data for the backend implementation (it is fully opaque)
    uint8_t m_impl[40]{0};
    //store a pointer to the backend implementation 
    void* m_backend = nullptr;

};

#endif

#endif