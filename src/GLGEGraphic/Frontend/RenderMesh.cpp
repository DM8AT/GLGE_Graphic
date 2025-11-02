/**
 * @file RenderMesh.cpp
 * @author DM8AT
 * @brief implement a render mesh
 * @version 0.1
 * @date 2025-11-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//add the render mesh frontend API
#include "RenderMesh.h"
//add debugging
#include "../../GLGE_BG/Debugging/Logging/__BG_SimpleDebug.h"

RenderMesh::RenderMesh(Mesh* mesh, Material* material) noexcept
 : m_mesh(mesh), m_material(material)
{
    //sanity check the combination
    GLGE_DEBUG_ASSERT("Incompatible layouts for a mesh and a material when used in a render mesh", 
                      m_mesh->getVertexLayout() != m_material->getVertexLayout());
}