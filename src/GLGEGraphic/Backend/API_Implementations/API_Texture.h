/**
 * @file API_Texture.h
 * @author DM8AT
 * @brief define the virtual class for the texture binding
 * @version 0.1
 * @date 2025-10-30
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_GRAPHIC_BACKEND_API_IMPL_TEXTURE_
#define _GLGE_GRAPHIC_BACKEND_API_IMPL_TEXTURE_

//only available for C++
#if __cplusplus

//add a vector as a dynamic resizable array
#include <vector>
//add a mutex for thread safety
#include <mutex>
#include <shared_mutex>

//add the frontend texture class
#include "../../Frontend/Texture.h"

//use the GLGE::Graphic::Backend::API namespace
namespace GLGE::Graphic::Backend::API
{

/**
 * @brief define the abstract texture API class
 */
class Texture
{
public:

    /**
     * @brief Construct a new Texture
     * 
     * @param tex the actual front-end texture
     * @param filterMode defines how the pixel values are interpolated between the texels
     * @param anisotropy defines the maximum value of anisotropy that can be used to sample from the texture
     * @param samples the amount of samples to take per pixel (default is 1 sample per pixel)
     * @param tiling the tiling mode for the texture
     */
    Texture(::Texture* tex, FilterMode filterMode, float anisotropy, TextureMultiSample samples, TextureTileMode tiling)
     : m_texture(tex), m_filterMode(filterMode), m_requested_filterMode(filterMode), 
       m_anisotropy(anisotropy) , m_requested_anisotropy(anisotropy), m_samples(samples), m_requested_samples(samples),
       m_tiling(tiling), m_requested_tiling(tiling)
    {}

    /**
     * @brief Set the Filter Mode of the texture
     * 
     * @param mode the new filtering mode for the texture
     */
    virtual void setFilterMode(FilterMode mode) noexcept = 0;

    /**
     * @brief Get the Filter Mode of the texture
     * 
     * @return TextureFilterMode the current filter mode of the texture
     */
    inline FilterMode getFilterMode() const noexcept {return m_filterMode;}

    /**
     * @brief check if the texture has a type that specifies it as a color texture
     * 
     * @return true : the texture is of a color type
     * @return false : the texture is not of a color type
     */
    inline bool isColor() const noexcept {return m_texture->isColor();}

    /**
     * @brief check if the texture is a depth stencil texture
     * 
     * @return true : the texture is a depth stencil texture
     * @return false : the texture is not a depth stencil texture
     */
    inline bool isDepthStencil() const noexcept {return m_texture->isDepthStencil();}

    /**
     * @brief check if the texture is a depth texture
     * 
     * @return true : the texture is a depth texture
     * @return false : the texture is a depth texture
     */
    inline bool isDepth() const noexcept {return m_texture->isDepth();}

    /**
     * @brief check if the texture is a stencil texture
     * 
     * @return true : the texture is a stencil texture
     * @return false : the texture is not a stencil texture
     */
    inline bool isStencil() const noexcept {return m_texture->isStencil();}

    /**
     * @brief Set the maximum anisotropy level for the texture
     * 
     * @param anisotropy the new maximum anisotropy level for the texture
     */
    virtual void setAnisotropy(float anisotropy) noexcept = 0;

    /**
     * @brief Get the requested Anisotropy level for the texture
     * 
     * @return float the requested anisotropy level of the texture
     */
    inline float getAnisotropy() const noexcept {return m_anisotropy;}

    /**
     * @brief Set the amount of samples to take per pixel for the texture
     * 
     * @param samples the amount of samples to take
     */
    virtual void setMultiSample(TextureMultiSample samples) noexcept = 0;

    /**
     * @brief Get the amount of samples the texture takes per pixel
     * 
     * @return TextureMultiSample the amount of samples taken per pixel
     */
    inline TextureMultiSample getMultiSample() {return m_samples;}

    /**
     * @brief Set the Tiling Mode for the texture
     * 
     * @param mode the new tiling mode of the texture
     */
    virtual void setTilingMode(TextureTileMode mode) noexcept = 0;

    /**
     * @brief Get the Tiling Mode of the texture
     * 
     * @return TextureTileMode the tiling mode of the texture
     */
    inline TextureTileMode getTilingMode() const noexcept {return m_tiling;}

    /**
     * @brief mark that this class is dirty (new CPU data)
     */
    virtual void markDirty() noexcept = 0;

    /**
     * @brief notify the api texture for a resizing
     */
    virtual void notifySizeChange() noexcept = 0;

protected:

    /**
     * @brief a function to tick the texture from the GPU 
     * 
     * @warning this function is only allowed to be called from the main thread during the global main tick
     */
    virtual void tickGPU() noexcept = 0;
    
    /**
     * @brief add the API instance as a friend class
     */
    friend class Instance;

    //store a pointer to the owning front-end texture
    ::Texture* m_texture = nullptr;
    //store the own filter mode
    FilterMode m_filterMode = GLGE_FILTER_MODE_LINEAR;
    //store the own anisotropic filter mode
    float m_anisotropy = 0.f;
    //store how many samples to take per pixel
    TextureMultiSample m_samples = GLGE_TEXTURE_SAMPLE_X1;
    //store how the texture is tiled
    TextureTileMode m_tiling = GLGE_TEXTURE_TILE_CLAMP;
    //store the currently requested filter mode
    FilterMode m_requested_filterMode = GLGE_FILTER_MODE_LINEAR;
    //store the own anisotropic filter mode
    float m_requested_anisotropy = 0.f;
    //store how many samples to take per pixel
    TextureMultiSample m_requested_samples = GLGE_TEXTURE_SAMPLE_X1;
    //store how to tile the texture
    TextureTileMode m_requested_tiling = GLGE_TEXTURE_TILE_CLAMP;

    /**
     * @brief store a mutex to make GPU updates thread safe
     */
    inline static std::shared_mutex m_updateMutex;
    /**
     * @brief store a list of all backend textures that want to be ticked
     */
    inline static std::vector<Texture*> m_toUpdate;

};

}

#endif

#endif