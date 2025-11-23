/**
 * @file API_Framebuffer.h
 * @author DM8AT
 * @brief define the API for framebuffer backends
 * @version 0.1
 * @date 2025-11-21
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//header guard
#ifndef _GLGE_GRAPHIC_BACKEND_API_IMPL_FRAMEBUFFER_
#define _GLGE_GRAPHIC_BACKEND_API_IMPL_FRAMEBUFFER_

//only available for C++
#if __cplusplus

//frontend framebuffers will be defined later
class Framebuffer;

//use the GLGE::Graphic::Backend::API namespace
namespace GLGE::Graphic::Backend::API
{

/**
 * @brief define the common API for all framebuffer backends
 */
class Framebuffer {
public:

    /**
     * @brief Construct a new Framebuffer
     * 
     * @param fbuff a pointer to the frontend framebuffer this backend belongs to
     */
    Framebuffer(::Framebuffer* fbuff) : m_fbuff(fbuff) {}

    /**
     * @brief Destroy the Framebuffer
     */
    virtual ~Framebuffer() = default;

    /**
     * @brief Get the Frontend
     * 
     * @return ::Framebuffer* a pointer to the frontend
     */
    inline ::Framebuffer* getFrontend() const noexcept {return m_fbuff;}

    /**
     * @brief re-create the framebuffer backend
     */
    virtual void recreate() noexcept = 0;

protected:

    //store the frontend framebuffer this backend belongs to
    ::Framebuffer* m_fbuff = nullptr;

};

}

#endif

#endif