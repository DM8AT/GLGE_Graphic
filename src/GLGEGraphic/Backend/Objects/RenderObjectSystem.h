/**
 * @file RenderObjectSystem.h
 * @author DM8AT
 * @brief define a system that uses render objects to give each object with a renderer component a unique id
 * @version 0.1
 * @date 2025-11-10
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//header guard
#ifndef _GLGE_GRAPHIC_BACKEND_OBJECTS_RENDER_OBJECT_SYSTEM_
#define _GLGE_GRAPHIC_BACKEND_OBJECTS_RENDER_OBJECT_SYSTEM_

//add types
#include "../../../GLGE_Core/Types.h"
//add frontend structured buffers
#include "../../Frontend/StructuredBuffer.h"
//add the compressed transforms
#include "CompressedTransform.h"

//define a constant to be used as a bitmask for the index
#define GLGE_RENDER_OBJECT_HANDLE_INDEX 0x3FFFFFu
//define the offset of the version data in bits
#define GLGE_RENDER_OBJECT_HANDLE_VERSION_OFFSET 22
//define a constant to be used as a bitmask for the version
#define GLGE_RENDER_OBJECT_HANDLE_VERSION 0xFFC00000u

//only available for C++
#if __cplusplus

//add vectors for a free list
#include <vector>
//add a mutex to make the system thread safe
#include <mutex>
#include <shared_mutex>
//add optionals to deferr construction
#include <optional>

//use a custom namespace for the backend: GLGE::Graphic::Backend
namespace GLGE::Graphic::Backend {

//use uint32_t's for object handles
typedef uint32_t RenderObjectHandle;

/**
 * @brief a class to hold a Render Object System
 * 
 * This is a system to give unique handles to all objects
 */
class RenderObjectSystem final
{
public:

    /**
     * @brief create a render object handle for a new render object
     * 
     * @return RenderObjectHandle the new handle for that object
     */
    static RenderObjectHandle create() noexcept;

    /**
     * @brief clean up a render object handle from an object that is being destroyed
     * 
     * @param handle the handle to delete
     */
    static void destroy(RenderObjectHandle handle) noexcept;

    /**
     * @brief a quick function to check if a handle is valid
     * 
     * @param handle the handle to check
     * @return true : the handle is valid
     * @return false : the handle is not valid
     */
    inline static bool isValid(RenderObjectHandle handle) {
        //thread safety
        std::shared_lock lock(m_mutex);
        //if the size is too small, stop
        if ((handle & GLGE_RENDER_OBJECT_HANDLE_INDEX) < m_handles.size()) {
            //sanity check if the version is correct
            return (handle & GLGE_RENDER_OBJECT_HANDLE_VERSION) == (m_handles[handle & GLGE_RENDER_OBJECT_HANDLE_INDEX] & GLGE_RENDER_OBJECT_HANDLE_VERSION);
        }
        return false;
    }

    /**
     * @brief Get the Transform Buffer
     * 
     * @return Buffer* a pointer to the transform structure
     */
    inline static StructuredBuffer<CompressedTransform>* getTransformBuffer() noexcept 
    {return (StructuredBuffer<CompressedTransform>*)&m_transfBuffStorage;}

protected:

    //store a list of all active handles
    inline static std::vector<RenderObjectHandle> m_handles;
    //store a list of free index entries to currently unused handles
    inline static std::vector<uint32_t> m_freeList;
    //store a mutex to make the render object system thread safe
    inline static std::shared_mutex m_mutex;

    //store a trippe-buffered structured buffer of compressed transforms
    inline static uint8_t m_transfBuffStorage[sizeof(StructuredBuffer<CompressedTransform>)]{ 0 };
    inline static StructuredBuffer<CompressedTransform>* m_transfBuff = nullptr;

};

}

#endif

#endif