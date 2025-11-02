/**
 * @file OGL_Texture.h
 * @author DM8AT
 * @brief define what an OpenGL texture is
 * @version 0.1
 * @date 2025-10-30
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_GRAPHIC_BACKEND_API_IMPL_OGL_TEXTURE_
#define _GLGE_GRAPHIC_BACKEND_API_IMPL_OGL_TEXTURE_

//only available for C++
#if __cplusplus

//add the types
#include "../../../../GLGE_Core/Types.h"

//add the texture API
#include "../API_Texture.h"
//atomics are used for syncing
#include <atomic>

//use the namespace GLGE::Graphic::Backend::OGL
namespace GLGE::Graphic::Backend::OGL
{

/**
 * @brief define a class to handle the backend for OpenGL textures
 */
class Texture final : public API::Texture
{
public:

    /**
     * @brief Construct a new Texture
     * 
     * @param tex a pointer to the frontend texture to create from
     */
    Texture(::Texture* tex);

    /**
     * @brief mark the data stored in the GPU texture as dirty
     */
    virtual void markDirty() noexcept override;

    /**
     * @brief notify the api texture for a resizing
     */
    virtual void notifySizeChange() noexcept;

    /**
     * @brief Get the Texture
     * 
     * @return uint32_t the OpenGL texture
     */
    inline uint32_t getTexture() const noexcept {return m_glTex;}

protected:

    /**
     * @brief tick the GPU data
     */
    virtual void tickGPU() noexcept override;

    /**
     * @brief store the flag that is used for re-creation / creation of the texture
     */
    inline static constexpr uint64_t FLAG_RECREATE = 0b1;

    /**
     * @brief store the flag that is used to re-size the texture
     */
    inline static constexpr uint64_t FLAG_RESIZE = 0b1;

    /**
     * @brief store if the element is queued
     */
    std::atomic<bool> m_queued;

    /**
     * @brief queue this texture for an update
     */
    void enqueue() noexcept;

    /**
     * @brief store the flags of all that should be updated on the texture
     */
    std::atomic<uint64_t> m_dirtFlags = 0;

    /**
     * @brief store the OpenGL texture identifier (0 = undefined)
     */
    uint32_t m_glTex = 0;

    /**
     * @brief re-create the texture
     */
    void recreate() noexcept;
    /**
     * @brief resize the texture
     */
    void resize() noexcept;

};

}

#endif

#endif