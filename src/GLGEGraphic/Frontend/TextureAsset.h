/**
 * @file TextureAsset.h
 * @author DM8AT
 * @brief define what an asset means for a texture to also be an asset
 * @version 0.1
 * @date 2025-10-31
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_GRAPHIC_FRONTEND_TEXTURE_ASSET_
#define _GLGE_GRAPHIC_FRONTEND_TEXTURE_ASSET_

//classes only available for C++
#if __cplusplus

//add textures
#include "Texture.h"
//add the asset system
#include "../../GLGE_Core/Assets/Assets.h"

/**
 * @brief store a class that is used to load textures from asset (texture) files
 */
class TextureAsset final : public BaseAsset
{
public:

    /**
     * @brief Construct a new Texture Asset
     * 
     * @param path the path to the texture to load
     * @param hdr specify if the texture asset should be loaded as an HDR texture or a "normal" texture
     * @param type the type for the GPU data
     * @param filterMode define the filter mode to use for the texture
     * @param maxAnisotropy define the maximum anisotropy value that can be used for sampling the texture
     */
    TextureAsset(const String& path, bool hdr, const TextureType& type, TextureFilterMode filterMode = TEXTURE_FILTER_MODE_LINEAR, float maxAnisotropy = 0.f)
     : m_path(path), m_hdr(hdr), m_type(type), m_filterMode(filterMode), m_anisotropy(maxAnisotropy)
    {}

    /**
     * @brief Destroy the Texture Asset
     */
    ~TextureAsset();

    /**
     * @brief Get the Texture loaded by the asset
     * 
     * @return Texture* a pointer to the loaded texture or NULL if the texture is not loaded
     */
    inline Texture* getTexture() noexcept {return m_ptr;}

    /**
     * @brief the function to load the actual asset
     */
    virtual void load() noexcept override;

protected:

    //store the actual texture
    uint8_t m_tex[sizeof(Texture)] = {0};
    //store a pointer to the texture
    //if the texture is not loaded, this is null
    Texture* m_ptr = nullptr;
    //store the path to the file to load
    String m_path = "";
    //store if the texture should load as HDR or not
    bool m_hdr = false;
    //store the actual image data
    TextureStorage m_storage = {
        .extent = uivec2(0,0),
        .isHDR = false,
        .channels = 0,
        .data{.n_hdr_1{0}}
    };
    //store the texture type
    TextureType m_type = GLGE_TEXTURE_RGBA;
    //store the filter mode
    TextureFilterMode m_filterMode = TEXTURE_FILTER_MODE_LINEAR;
    //store the requested maximum anisotropy
    float m_anisotropy = 0.f;

};

#endif

#endif