/**
 * @file API_Instance.h
 * @author DM8AT
 * @brief define the base class for all graphic instances
 * @version 0.1
 * @date 2025-10-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_GRAPHIC_BACKEND_API_IMPL_API_INSTANCE_
#define _GLGE_GRAPHIC_BACKEND_API_IMPL_API_INSTANCE_

//only available for C++
#if __cplusplus

//add memory arenas
#include "API_MemoryArena.h"

//use the GLGE::Graphic::Backend::API namespace
namespace GLGE::Graphic::Backend::API
{

/**
 * @brief define the base class for all graphic APIs
 */
class Instance
{
public:

    /**
     * @brief Construct a new Instance
     * 
     * @param vbuff a pointer to the abstract vertex Memory Arena
     * @param ibuff a pointer to the abstract index Memory Arena
     */
    Instance(API::MemoryArena* vbuff, API::MemoryArena* ibuff)
     : m_abs_vertexBuffer(vbuff), m_abs_indexBuffer(ibuff)
    {}

    /**
     * @brief Destroy the Instance
     */
    virtual ~Instance() {}

    /**
     * @brief tick the whole graphic stuff
     */
    void tick() noexcept;

    /**
     * @brief update the own instance
     */
    virtual void onUpdate() noexcept = 0;

    /**
     * @brief Get the Vertex Memory Arena of the instance
     * 
     * @return `API::MemoryArena*` a pointer to the vertex buffer
     */
    inline API::MemoryArena* getVertexBuffer() noexcept {return m_abs_vertexBuffer;}

    /**
     * @brief Get the Index Memory Arena of the instance
     * 
     * @return `API::MemoryArena*` a pointer to the index buffer
     */
    inline API::MemoryArena* getIndexBuffer() noexcept {return m_abs_indexBuffer;}

protected:

    //store a pointer to the abstract vertex buffer
    API::MemoryArena* m_abs_vertexBuffer = nullptr;
    //store a pointer to the abstract index buffer
    API::MemoryArena* m_abs_indexBuffer = nullptr;

};

};

#endif

#endif