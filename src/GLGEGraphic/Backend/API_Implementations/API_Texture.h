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
     */
    Texture(::Texture* tex, TextureFilterMode filterMode, float anisotropy)
     : m_texture(tex), m_filterMode(filterMode), m_requested_filterMode(filterMode), 
       m_anisotropy(anisotropy) , m_requested_anisotropy(anisotropy)
    {}

    /**
     * @brief Set the Filter Mode of the texture
     * 
     * @param mode the new filtering mode for the texture
     */
    virtual void setFilterMode(TextureFilterMode mode) noexcept = 0;

    /**
     * @brief Get the Filter Mode of the texture
     * 
     * @return TextureFilterMode the current filter mode of the texture
     */
    inline TextureFilterMode getFilterMode() const noexcept {return m_filterMode;}

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
    TextureFilterMode m_filterMode = TEXTURE_FILTER_MODE_LINEAR;
    //store the own anisotropic filter mode
    float m_anisotropy = 0.f;
    //store the currently requested filter mode
    TextureFilterMode m_requested_filterMode = TEXTURE_FILTER_MODE_LINEAR;
    //store the own anisotropic filter mode
    float m_requested_anisotropy = 0.f;

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