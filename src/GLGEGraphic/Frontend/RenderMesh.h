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
#ifndef _GLGE_GRAPHIC_FRONTEND_RENDER_MESH_
#define _GLGE_GRAPHIC_FRONTEND_RENDER_MESH_

//for C++ define a class
#if __cplusplus

//add the core meshes
#include "../../GLGE_Core/Geometry/Surface/Mesh.h"
//also add materials
#include "Material.h"

/**
 * @brief define the frontend API for a render mesh
 */
class RenderMesh
{
public:

    /**
     * @brief Construct a new Render Mesh
     * 
     * @param mesh the core mesh to encapsulate
     * @param material the material to use for the mesh
     */
    RenderMesh(Mesh* mesh, Material* material) noexcept;

protected:

    //store a pointer to the core mesh
    Mesh* m_mesh = nullptr;
    //store a pointer to the backend implementation 
    void* m_backend = nullptr;
    //store a pointer to the used material
    Material* m_material = nullptr;

};

#endif

#endif