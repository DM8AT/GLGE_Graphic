/**
 * @file OGL_Framebuffer.h
 * @author DM8AT
 * @brief define the OpenGL implementation for a framebuffer
 * @version 0.1
 * @date 2025-11-21
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//header guard
#ifndef _GLGE_GRAPHIC_BACKEND_API_IMPL_OGL_FRAMEBUFFER_
#define _GLGE_GRAPHIC_BACKEND_API_IMPL_OGL_FRAMEBUFFER_

//add the API
#include "../API_Framebuffer.h"
//add types
#include "../../../../GLGE_Core/Types.h"

//only available for C++
#if __cplusplus

//add vectors for queues
#include <vector>

//use the namespace GLGE::Graphic::Backend::OGL
namespace GLGE::Graphic::Backend::OGL
{

/**
 * @brief define the OpenGL framebuffer
 */
class Framebuffer : public API::Framebuffer {
public:

    /**
     * @brief Construct a new Framebuffer
     * 
     * @param fbuff a pointer to the frontend framebuffer to create from
     */
    Framebuffer(::Framebuffer* fbuff);

    /**
     * @brief Destroy the Framebuffer
     */
    virtual ~Framebuffer();

    /**
     * @brief get the OpenGL framebuffer object from the framebuffer
     * 
     * @return uint32_t the OpenGL framebuffer object
     */
    inline uint32_t getFBO() const noexcept {return m_fbo;}

    /**
     * @brief re-create the framebuffer backend
     */
    virtual void recreate() noexcept override;

protected:

    //store the OpenGL framebuffer object
    uint32_t m_fbo = 0;

    //create the framebuffer
    void create() noexcept;

    //clean up the framebuffer
    static void destroy(uint32_t fbo) noexcept;

    //a function to call to recreate the backend
    void recreate_backend() noexcept;

    //add the instance as a friend class
    friend class Instance;

    //store a structure to push to the framebuffer creation / clean up
    struct GL_Handle {
        //store a pointer to this framebuffer
        Framebuffer* self;
        //store the OpenGL handle
        uint32_t fbo;
    };

    //store a queue that is used to initialise / de-initialise framebuffers on the main thread
    inline static std::vector<GL_Handle> s_De_InitQueue;

};

}

#endif

#endif