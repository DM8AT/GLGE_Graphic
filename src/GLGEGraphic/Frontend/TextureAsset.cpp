/**
 * @file TextureAsset.cpp
 * @author DM8AT
 * @brief implement the texture asset loading
 * @version 0.1
 * @date 2025-10-31
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//add the texture asset
#include "TextureAsset.h"

//add STB image with the implementation
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void TextureAsset::load() noexcept
{
    //define the texture as loading
    updateLoadState(ASSET_STATE_LOADING);

    //store the dimensions of the texture
    ivec2 extent;
    //store the channel count
    int channels = 0;
    //parse the texture data using STB image
    if (m_hdr) {
        //actual file parsing
        float* data = stbi_loadf(m_path.c_str(), &extent.x, &extent.y, &channels, 0);
        GLGE_ASSERT("Failed to load texture file " << m_path, !data);

        //store the data in the correct way
        m_storage.isHDR = true;
        m_storage.channels = channels;
        m_storage.extent = uivec2(extent.x, extent.y);
        switch (m_storage.channels)
        {
        case 1:
            m_storage.data.hdr_1 = data;
            break;
        case 2:
            m_storage.data.hdr_2 = (vec2*)data;
            break;
        case 3:
            m_storage.data.hdr_3 = (vec3*)data;
            break;
        case 4:
            m_storage.data.hdr_4 = (vec4*)data;
            break;
        
        default:
            //error
            stbi_image_free(data);
            *((void**)&m_storage.data) = nullptr;
            GLGE_ABORT("Failed to parse texture " << m_path << " - unsupported number of channels: " << m_storage.channels);
            break;
        }
    } else {
        //actual file parsing
        uint8_t* data = stbi_load(m_path.c_str(), &extent.x, &extent.y, &channels, 0);
        GLGE_ASSERT("Failed to load texture file " << m_path, !data);

        //store the data in the correct way
        m_storage.isHDR = false;
        m_storage.channels = channels;
        m_storage.extent = uivec2(extent.x, extent.y);
        switch (m_storage.channels)
        {
        case 1:
            m_storage.data.n_hdr_1 = data;
            break;
        case 2:
            m_storage.data.n_hdr_2 = (uint16_t*)data;
            break;
        case 3:
            m_storage.data.n_hdr_3 = (uint24_t*)data;
            break;
        case 4:
            m_storage.data.n_hdr_4 = (uint32_t*)data;
            break;
        
        default:
            //error
            stbi_image_free(data);
            *((void**)&m_storage.data) = nullptr;
            GLGE_ABORT("Failed to parse texture " << m_path << " - unsupported number of channels: " << m_storage.channels);
            break;
        }
    }

    //create the actual texture
    m_ptr = new (m_tex) Texture(m_storage, m_type, m_filterMode, m_anisotropy);

    //loading finished
    updateLoadState(ASSET_STATE_LOADED);
}

TextureAsset::~TextureAsset()
{
    //if the data exists, free it
    if (*((void**)&m_storage.data)) {
        stbi_image_free(*((void**)&m_storage.data));
        *((void**)&m_storage.data) = nullptr;
    }
}