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
#include "../../../GLGE_BG/Debugging/Logging/__BG_SimpleDebug.h"
//add the API
#include "../../Backend/API_Implementations/API_RenderMesh.h"

RenderMesh::RenderMesh(Mesh* mesh) noexcept
 : m_mesh(mesh)
{
    //sanity check
    static_assert(sizeof(m_impl) == sizeof(GLGE::Graphic::Backend::API::RenderMesh), "Invalid size for the render mesh data storage");
    //create the API implementation
    m_backend = new (m_impl) GLGE::Graphic::Backend::API::RenderMesh(this);
}

RenderMesh::~RenderMesh() noexcept
{
    if (m_backend) {
        ((GLGE::Graphic::Backend::API::RenderMesh*)m_backend)->~RenderMesh();
        m_backend = nullptr;
    }
}