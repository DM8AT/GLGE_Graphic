/**
 * @file Texture.cpp
 * @author DM8AT
 * @brief implement the texture-stuff and STB-image
 * @version 0.1
 * @date 2025-10-21
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//include the texture
#include "Texture.h"

//include the file stuff
#include <filesystem>
#include <fstream>

//add the backend instance
#include "../Backend/Instance.h"
//also add the full backend API
#include "../Backend/API_Implementations/AllImplementations.h"

//add debugging stuff
#include "../../GLGE_BG/Debugging/Logging/__BG_SimpleDebug.h"

Texture::Texture(const TextureStorage& storage, TextureType type, FilterMode filterMode, float anisotropy, TextureMultiSample samples)
 : m_texStorage(storage), m_type(type)
{
    //create the backend texture
    switch (GLGE::Graphic::Backend::INSTANCE.getAPI())
    {
    case GLGE_GRAPHIC_INSTANCE_API_OPEN_GL:
        m_tex = new GLGE::Graphic::Backend::OGL::Texture(this, filterMode, anisotropy, samples);
        break;
    
    default:
        break;
    }
}

void Texture::resizeAndClear(const uivec2& size) noexcept
{
    //clear all the stored data (set to NULL)
    *((void**)&m_texStorage.data) = nullptr;
    m_texStorage.extent = size;
    //notify the backend texture of the size change
    ((GLGE::Graphic::Backend::API::Texture*)m_tex)->notifySizeChange();
}

void Texture::setFilterMode(FilterMode mode) noexcept {
    //pass through the call
    ((GLGE::Graphic::Backend::API::Texture*)m_tex)->setFilterMode(mode);
}

FilterMode Texture::getFilterMode() const noexcept {
    //pass through the call
    return ((GLGE::Graphic::Backend::API::Texture*)m_tex)->getFilterMode();
}

void Texture::setAnisotropy(float anisotropy) noexcept {
    //pass through the call
    ((GLGE::Graphic::Backend::API::Texture*)m_tex)->setAnisotropy(anisotropy);
}

float Texture::getAnisotropy() const noexcept {
    //pass through the call
    return ((GLGE::Graphic::Backend::API::Texture*)m_tex)->getAnisotropy();
}

bool Texture::isColor() const noexcept {
    //just switch over the hard-coded type mask
    switch (m_type)
    {
    case GLGE_TEXTURE_R:
    case GLGE_TEXTURE_RG:
    case GLGE_TEXTURE_RGB:
    case GLGE_TEXTURE_RGBA:
    case GLGE_TEXTURE_R_F:
    case GLGE_TEXTURE_RG_F:
    case GLGE_TEXTURE_RGB_F:
    case GLGE_TEXTURE_RGBA_F:
    case GLGE_TEXTURE_R_H:
    case GLGE_TEXTURE_RG_H:
    case GLGE_TEXTURE_RGB_H:
    case GLGE_TEXTURE_RGBA_H:
        return true;
        break;
    
    default:
        return false;
        break;
    }
}

bool Texture::isDepthStencil() const noexcept {
    //just switch over the hard-coded type mask
    switch (m_type)
    {
    case GLGE_TEXTURE_DEPTH_24_STENCIL_8:
    case GLGE_TEXTURE_DEPTH_32_STENCIL_8:
        return true;
        break;
    
    default:
        return false;
        break;
    }
}

bool Texture::isDepth() const noexcept {
    //just switch over the hard-coded type mask
    switch (m_type)
    {
    case GLGE_TEXTURE_DEPTH_16:
    case GLGE_TEXTURE_DEPTH_24:
    case GLGE_TEXTURE_DEPTH_32:
        return true;
        break;
    
    default:
        return false;
        break;
    }
}

bool Texture::isStencil() const noexcept {
    //always false
    return false;
}


//special printing stuff
#include <iomanip>
#include <algorithm> //std::clamp

/**
 * @brief a helper function to make all the HDR values to low range
 * 
 * @param v the value to convert to normalized range
 * @return uint32_t the value from 0 to 255 that identifies the color value
 */
static uint32_t to_byte(float v) {
    return (uint32_t)(std::clamp(v, 0.f, 1.f) * 255.f);
}

/**
 * @brief Get the Color of a single pixel
 * 
 * @param i the index of the pixel in the image
 * @param data the data to read the pixel from
 * @param pixel a pointer to fill with the pixel data in the format (uint8_t r, uint8_t g, uint8_t b)
 */
static void getColor(uint64_t i, const TextureStorage& data, uint8_t* pixel) noexcept {
    //extract channel infos
    const bool hdr = data.isHDR;
    const uint8_t ch = data.channels;
    const uint8_t iterCount = (ch > 3) ? 3 : ch;

    //value HDR pixels independent
    if (hdr)
    {
        //store a temporary float value
        vec3 pix = 0;
        //interpret the value as a float[]
        for (uint8_t j = 0; j < iterCount; ++j) 
        {pix.vals[j] = (*((float**)&data.data))[i * ch + j];}

        //apply some simple tone-mapping (ACES approximation)
        pix = (pix*(pix*2.51 + 0.03)) / (pix*(pix*2.43 + 0.59) + 0.14);

        //gamma correction
        for (uint8_t j = 0; j < iterCount; ++j)
        {pix.vals[j] = std::pow(std::clamp(pix.vals[j],0.f,1.f), 1./2.2);}

        //store the final color
        for (uint8_t j = 0; j < iterCount; ++j)
        {pixel[j] = (uint8_t)(std::clamp(pix.vals[j], 0.f, 1.f) * 255.f);}
    }
    else
    {
        //non-HDR
        for (uint8_t j = 0; j < iterCount; ++j) 
        {pixel[j] = (*((uint8_t**)&data.data))[i * ch + j];}
    }

    //fill grey scale
    for (uint8_t j = iterCount; j < 3; ++j)
    {pixel[j] = pixel[iterCount - 1];}
}

std::ostream& operator<<(std::ostream& os, const Texture& tex) noexcept
{
    //get access to the texture storage
    const TextureStorage& data = tex.m_texStorage;

    //make sure that the texture exists
    if (data.extent.x == 0 || data.extent.y == 0 || data.channels == 0) {
        os << "Empty texture.\n";
        return os;
    }
    //sanity check the data
    if (!data.data.hdr_1) {
        //GPU only data
        return (os << "GPU Only texture\n");
    }

    //get access to the important data (like dimensions)
    const uint32_t width = data.extent.x;
    const uint32_t height = data.extent.y;

    //reset all flags
    os << std::resetiosflags(std::ios::basefield);

    //iterate over all pixels
    //because each character is two pixels, increase y by two per iteration
    for (uint32_t y = 0; y < height; y += 2)
    {
        //iterate over all pixels in the row
        for (uint32_t x = 0; x < width; ++x)
        {
            //get the top pixel. If the element is not stored, default to black (0)
            //store two pixels because two pixels per iteration are printed
            uint8_t lower[3]{ 0 }, upper[3]{ 0 };

            //get the color for both
            getColor(y * width + x, data, lower);
            //check if the pixel would still be in range. Inly then get the color. 
            if (height > y+1) {
                getColor((y + 1) * width + x, data, upper);
            }

            //print top pixel as foreground, bottom as background
            os << "\x1b[38;2;" << (int)lower[0] << ';' << (int)lower[1] << ';' << (int)lower[2] << 'm'
               << "\x1b[48;2;" << (int)upper[0] << ';' << (int)upper[1] << ';' << (int)upper[2] << 'm'
               << "▀";
        }
        //ensure a new-line character
        std::cout << "\x1b[0m\n";
    }
    
    //ensure reset at end
    os << "\x1b[0m";
    return os;
}