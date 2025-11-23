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
     * @param filterMode defines how the pixel values are interpolated between the texels
     * @param anisotropy defines the maximum value of anisotropy that can be used to sample from the texture
     * @param samples the amount of samples to take per pixel (default is 1 sample per pixel)
     * @param tiling the tiling mode for the texture
     */
    Texture(::Texture* tex, FilterMode filterMode, float anisotropy, TextureMultiSample samples, TextureTileMode tiling);

    /**
     * @brief Set the Filter Mode of the texture
     * 
     * @param mode the new filtering mode for the texture
     */
    virtual void setFilterMode(FilterMode mode) noexcept override;

    /**
     * @brief Set the maximum anisotropy level for the texture
     * 
     * @param anisotropy the new maximum anisotropy level for the texture
     */
    virtual void setAnisotropy(float anisotropy) noexcept override;

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

    /**
     * @brief Get the Texture Type (texture 2D if multi-sampling is disabled, else texture 2D multisample)
     * 
     * @return uint32_t the OpenGL enum value
     */
    inline uint32_t getTextureType() const noexcept {return (m_samples == GLGE_TEXTURE_SAMPLE_X1) ? 0x0DE1 : 0x9100;}

    /**
     * @brief Set the amount of samples to take per pixel for the texture
     * 
     * @param samples the amount of samples to take
     */
    virtual void setMultiSample(TextureMultiSample samples) noexcept override;

    /**
     * @brief Set the Tiling Mode for the texture
     * 
     * @param mode the new tiling mode of the texture
     */
    virtual void setTilingMode(TextureTileMode mode) noexcept override;

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
    inline static constexpr uint64_t FLAG_RESIZE = 0b10;

    /**
     * @brief store the flags that is used to state that the filter state is out of date
     */
    inline static constexpr uint64_t FLAG_UPDATE_FILTER = 0b100;

    /**
     * @brief store the flags that is used to state that the anisotropic filter state is out of date
     */
    inline static constexpr uint64_t FLAG_UPDATE_ANISOTROPY = 0b1000;

    /**
     * @brief store the flags that is used to state that the anisotropic filter state is out of date
     */
    inline static constexpr uint64_t FLAG_UPDATE_SAMPLING = 0b1000;

    /**
     * @brief store the flags that is used to state that the tiling state is out of date
     */
    inline static constexpr uint64_t FLAG_UPDATE_TILING = 0b10000;

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

};

}

#endif

#endif