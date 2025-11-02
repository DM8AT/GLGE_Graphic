/**
 * @file OGL_Instance.h
 * @author DM8AT
 * @brief define the main instance for OpenGL rendering
 * @version 0.1
 * @date 2025-10-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_GRAPHIC_BACKEND_API_IMPL_OPEN_GL_INSTANCE_
#define _GLGE_GRAPHIC_BACKEND_API_IMPL_OPEN_GL_INSTANCE_

//only available for C++
#if __cplusplus

//include the base instance
#include "../API_Instance.h"
//add all types
#include "../../../../GLGE_Core/Types.h"
//add buffers
#include "OGL_Buffer.h"

//a window is required to create graphic stuff
class Window;

//use the namespace GLGE::Graphic::Backend::OGL
namespace GLGE::Graphic::Backend::OGL
{

/**
 * @brief define an OpenGL instance
 */
class Instance final : public GLGE::Graphic::Backend::API::Instance
{
public:

    /**
     * @brief store all loaded extensions
     */
    struct LoadedExtensions {
        bool int64 = false;
    };

    /**
     * @brief Construct a new Instance
     * 
     * @param window a pointer to the window that was just created
     */
    Instance(::Window* window);

    /**
     * @brief Destroy the Instance
     */
    virtual ~Instance();

    /**
     * @brief Get the OpenGL Context
     * 
     * @return void* a pointer to the OpenGL context
     */
    inline void* getContext() const noexcept {return m_glContext;}

    /**
     * @brief Get the Widnow Flags for an OpenGL window
     * 
     * @return uint32_t the minimum window flags required
     */
    static uint32_t getWidnowFlags() noexcept;

    /**
     * @brief tick the instance
     */
    virtual void onUpdate() noexcept override {}

    /**
     * @brief Get the loaded Extensions
     * 
     * @return const LoadedExtensions& a reference to all loaded extensions
     */
    inline const LoadedExtensions& getExtensions() const noexcept {return m_extensions;}

protected:

    /**
     * @brief store the OpenGL context
     */
    void* m_glContext = nullptr;

    //store the vertex buffer for the instance
    OGL::Buffer m_vertexBuffer{nullptr, 0, Buffer::Type::VERTEX_BUFFER};
    //store the index buffer for the instance
    OGL::Buffer m_indexBuffer{nullptr, 0, Buffer::Type::INDEX_BUFFER};

    //store the loaded extensions
    LoadedExtensions m_extensions;

};

};

#endif

#endif