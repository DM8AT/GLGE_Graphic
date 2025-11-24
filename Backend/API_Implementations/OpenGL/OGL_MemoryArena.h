/**
 * @file OGL_MemoryArena.h
 * @author DM8AT
 * @brief define the API for a memory arena for OpenGL
 * @version 0.1
 * @date 2025-11-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_GRAPHIC_BACKEND_API_IMPL_OGL_MEMORY_ARENA_
#define _GLGE_GRAPHIC_BACKEND_API_IMPL_OGL_MEMORY_ARENA_

//add the memory arena
#include "../API_MemoryArena.h"
//add OpenGL buffers
#include "OGL_Buffer.h"

//only available for C++
#if __cplusplus

//use the namespace GLGE::Graphic::Backend::OGL
namespace GLGE::Graphic::Backend::OGL
{

/**
 * @brief implement the memory arena for OpenGL
 */
class MemoryArena final : public API::MemoryArena
{
public:

    /**
     * @brief Construct a new Memory Arena
     * 
     * @param size the starting size of the memory arena
     * @param allowResize specify if the memory arena can change the size
     * @param type the type of buffer to use for the memory arena
     */
    MemoryArena(uint64_t size, bool allowResize, Buffer::Type type)
     : m_buffer(nullptr, 0, type), API::MemoryArena(size, allowResize, &m_buffer)
    {}

    /**
     * @brief Destroy the Memory Arena
     */
    virtual ~MemoryArena() = default;

protected:

    //store the buffer for the memory arena
    OGL::Buffer m_buffer;

};

}

#endif

#endif