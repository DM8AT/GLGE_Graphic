/**
 * @file API_RenderMesh.cpp
 * @author DM8AT
 * @brief implement the functionality of a render mesh
 * @version 0.1
 * @date 2025-11-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//add the API for a render mesh
#include "API_RenderMesh.h"
//add the graphic instance
#include "../Instance.h"
//add the frontend render mesh
#include "../../Frontend/RenderAPI/RenderMesh.h"

GLGE::Graphic::Backend::API::RenderMesh::RenderMesh(::RenderMesh* rMesh)
 : m_rMesh(rMesh), 
   m_vboPointer(Backend::INSTANCE.getInstance()->getVertexBuffer()->allocate(
       rMesh->getMesh()->getVertexCount() * rMesh->getMesh()->getVertexLayout().getVertexSize())),
   m_iboPointer(Backend::INSTANCE.getInstance()->getIndexBuffer()->allocate(
       rMesh->getMesh()->getIndexCount() * sizeof(index_t)))
{
    //write the data
    Backend::INSTANCE.getInstance()->getVertexBuffer()->update(m_vboPointer, m_rMesh->getMesh()->getVertices());
    Backend::INSTANCE.getInstance()->getIndexBuffer()->update(m_iboPointer, m_rMesh->getMesh()->getIndices());
}

GLGE::Graphic::Backend::API::RenderMesh::~RenderMesh()
{
    //if the instance was deleted, the GPU memory is allready freed
    if (!Backend::INSTANCE.getInstance()) {return;}
    //free the pointer
    Backend::INSTANCE.getInstance()->getVertexBuffer()->release(m_vboPointer);
    Backend::INSTANCE.getInstance()->getIndexBuffer()->release(m_iboPointer);
}