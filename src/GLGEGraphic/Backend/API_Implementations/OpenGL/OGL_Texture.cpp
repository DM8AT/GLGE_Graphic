/**
 * @file OGL_Texture.cpp
 * @author DM8AT
 * @brief implement the OpenGL texture
 * @version 0.1
 * @date 2025-10-30
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//include the OpenGL texture interface
#include "OGL_Texture.h"
//add the OpenGL command buffer
#include "OGL_CommandBuffer.h"
//access the frontend texture
#include "../../../Frontend/Texture.h"
//add OpenGL
#include "glad/glad.h"

/**
 * @brief convert a texture type enum to a texture format for OpenGL
 * 
 * @param type the value of the texture enum to convert
 * @return GLenum the texture format for OpenGL
 */
static GLenum getGLTextureFormat(TextureType type) noexcept
{
    switch (type)
    {
    case GLGE_TEXTURE_R:
        return GL_R8;
        break;
    case GLGE_TEXTURE_R_F:
        return GL_R32F;
        break;
    case GLGE_TEXTURE_R_H:
        return GL_R16F;
        break;
    case GLGE_TEXTURE_RG:
        return GL_RG8;
        break;
    case GLGE_TEXTURE_RG_F:
        return GL_RG32F;
        break;
    case GLGE_TEXTURE_RG_H:
        return GL_RG16F;
        break;
    case GLGE_TEXTURE_RGB:
        return GL_RGB8;
        break;
    case GLGE_TEXTURE_RGB_F:
        return GL_RGB32F;
        break;
    case GLGE_TEXTURE_RGB_H:
        return GL_RGB16F;
        break;
    case GLGE_TEXTURE_RGBA:
        return GL_RGBA8;
        break;
    case GLGE_TEXTURE_RGBA_F:
        return GL_RGBA32F;
        break;
    case GLGE_TEXTURE_RGBA_H:
        return GL_RGBA16F;
        break;

    case GLGE_TEXTURE_DEPTH_16:
        return GL_DEPTH_COMPONENT16;
        break;
    case GLGE_TEXTURE_DEPTH_24:
        return GL_DEPTH_COMPONENT24;
        break;
    case GLGE_TEXTURE_DEPTH_32:
        return GL_DEPTH_COMPONENT32F;
        break;
    case GLGE_TEXTURE_DEPTH_24_STENCIL_8:
        return GL_DEPTH24_STENCIL8;
        break;
    case GLGE_TEXTURE_DEPTH_32_STENCIL_8:
        return GL_DEPTH32F_STENCIL8;
        break;
    
    default:
        //just return null as error value
        return 0;
        break;
    }
}

/**
 * @brief get the layout (type-erased) format
 * 
 * @param type the texture type to quarry the layout for
 * @return GLenum the OpenGL layout corresponding to the texture type
 */
static GLenum getGLTextureLayout(TextureType type) noexcept
{
    switch (type)
    {
    case GLGE_TEXTURE_R:
    case GLGE_TEXTURE_R_H:
    case GLGE_TEXTURE_R_F:
        return GL_RED;
        break;
    case GLGE_TEXTURE_RG:
    case GLGE_TEXTURE_RG_H:
    case GLGE_TEXTURE_RG_F:
        return GL_RG;
        break;
    case GLGE_TEXTURE_RGB:
    case GLGE_TEXTURE_RGB_H:
    case GLGE_TEXTURE_RGB_F:
        return GL_RGB;
        break;
    case GLGE_TEXTURE_RGBA:
    case GLGE_TEXTURE_RGBA_H:
    case GLGE_TEXTURE_RGBA_F:
        return GL_RGBA;
        break;
    case GLGE_TEXTURE_DEPTH_16:
    case GLGE_TEXTURE_DEPTH_24:
    case GLGE_TEXTURE_DEPTH_32:
        return GL_DEPTH_COMPONENT;
        break;
    case GLGE_TEXTURE_DEPTH_24_STENCIL_8:
    case GLGE_TEXTURE_DEPTH_32_STENCIL_8:
        return GL_DEPTH_STENCIL;
        break;
    
    default:
        return 0;
        break;
    }
}

/**
 * @brief get the layout (type-erased) format
 * 
 * @param data a constant reference to the texture storage to get the texture layout
 * @return GLenum the OpenGL layout corresponding to the texture type
 */
static GLenum getGLTextureLayout(const TextureStorage& data) noexcept
{
    switch (data.channels)
    {
    case 1:
        return GL_RED;
        break;
    case 2:
        return GL_RG;
        break;
    case 3:
        return GL_RGB;
        break;
    case 4:
        return GL_RGBA;
        break;
    
    default:
        return 0;
        break;
    }
}

GLGE::Graphic::Backend::OGL::Texture::Texture(::Texture* tex, FilterMode filterMode, float anisotropy, TextureMultiSample samples)
 : API::Texture(tex, filterMode, anisotropy, samples) 
{
    //directly mark the texture data as dirty
    markDirty();
}

void GLGE::Graphic::Backend::OGL::Texture::setFilterMode(FilterMode mode) noexcept
{
    //store the new requested filter mode
    m_requested_filterMode = mode;
    //update the dirty flag
    m_dirtFlags.fetch_or(FLAG_UPDATE_FILTER, std::memory_order_acq_rel);
    //queue this texture
    enqueue();
}

void GLGE::Graphic::Backend::OGL::Texture::setAnisotropy(float anisotropy) noexcept
{
    //store the new requested anisotropy level
    m_requested_anisotropy = anisotropy;
    //update the dirty flag
    m_dirtFlags.fetch_or(FLAG_UPDATE_ANISOTROPY, std::memory_order_acq_rel);
    //queue this texture
    enqueue();
}

void GLGE::Graphic::Backend::OGL::Texture::markDirty() noexcept
{
    //store that the texture is dirty
    m_dirtFlags.fetch_or(FLAG_RECREATE, std::memory_order_acq_rel);
    //queue this texture
    enqueue();
}

void GLGE::Graphic::Backend::OGL::Texture::notifySizeChange() noexcept
{
    //store that the texture is dirty
    m_dirtFlags.fetch_or(FLAG_RESIZE, std::memory_order_acq_rel);
    //queue this texture
    enqueue();
}

void GLGE::Graphic::Backend::OGL::Texture::enqueue() noexcept
{
    //if the texture is allready enqueued, stop
    if (m_queued.load(std::memory_order_relaxed)) {return;}

    //queue the texture
    std::shared_lock lock(m_updateMutex);
    m_queued.store(true, std::memory_order_relaxed);
    m_toUpdate.push_back(this);
}

void GLGE::Graphic::Backend::OGL::Texture::tickGPU() noexcept
{
    //switch over the flags
    if (m_dirtFlags.load(std::memory_order_acquire) & FLAG_RECREATE) {
        recreate();
    } 
    if (m_dirtFlags.load(std::memory_order_acquire) & FLAG_RESIZE) {
        recreate();
    } 
    if (m_dirtFlags.load(std::memory_order_acquire) & FLAG_UPDATE_FILTER) {
        //set the new filter level
        glTextureParameteri(m_glTex, GL_TEXTURE_MIN_FILTER, (m_requested_filterMode == GLGE_FILTER_MODE_LINEAR) ? GL_LINEAR : GL_NEAREST);
        glTextureParameteri(m_glTex, GL_TEXTURE_MAG_FILTER, (m_requested_filterMode == GLGE_FILTER_MODE_LINEAR) ? GL_LINEAR : GL_NEAREST);
        //store the new filter mode
        m_filterMode = m_requested_filterMode;
    } 
    if (m_dirtFlags.load(std::memory_order_acquire) & FLAG_UPDATE_ANISOTROPY) {
        //if supported, enable / disable anisotropic filtering
        if (GLAD_GL_EXT_texture_filter_anisotropic) {
            //get the maximum supported anisotropic value
            float maxAnisotropy = 0.f;
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
            //set the anisotropy value
            glTextureParameterf(m_glTex, GL_TEXTURE_MAX_ANISOTROPY, (m_requested_anisotropy < maxAnisotropy) ? m_requested_anisotropy : maxAnisotropy);
            //store the new anisotropy level
            m_anisotropy = m_requested_anisotropy;
        }
    }

    //reset the flags
    m_dirtFlags.store(0, std::memory_order_relaxed);
    //this texture is no longer queued
    m_queued.store(false, std::memory_order_release);
}

void GLGE::Graphic::Backend::OGL::Texture::recreate() noexcept
{
    //check if the texture exists
    if (m_glTex)
    {
        //if it does, check if it needs to be re-created
        int32_t width = 0;
        int32_t height = 0;
        glGetTextureLevelParameteriv(m_glTex, 0, GL_TEXTURE_WIDTH, &width);
        glGetTextureLevelParameteriv(m_glTex, 0, GL_TEXTURE_HEIGHT, &height);
        if ((width == m_texture->getData().extent.x) && (height == m_texture->getData().extent.y))
        {
            //respect multi-sampling upload
            if (m_samples == GLGE_TEXTURE_SAMPLE_X1) {
                //can re-upload just the data, no re-creation needed
                glTextureSubImage2D(m_glTex, 0, 0,0, m_texture->getData().extent.x, m_texture->getData().extent.y, getGLTextureLayout(m_texture->getData()), 
                                    m_texture->getData().isHDR ? GL_FLOAT : GL_UNSIGNED_BYTE, *((void**)&m_texture->getData().data));
            } else {
                
            }
        }
        else
        {
            //data re-creation needed
            //first, clean up
            glDeleteTextures(1, &m_glTex);
        }   
    }

    //create the texture
    glCreateTextures(getTextureType(), 1, &m_glTex);
    GLenum format = getGLTextureFormat(m_texture->getType());
    //check the texture type for storage creation
    if (m_samples == GLGE_TEXTURE_SAMPLE_X1) {
        glTextureStorage2D(m_glTex, 1, format, m_texture->getData().extent.x, m_texture->getData().extent.y);
    } else {
        glTextureStorage2DMultisample(m_glTex, (m_samples == GLGE_TEXTURE_SAMPLE_X1_FORCE) ? 1 : (uint32_t)m_samples, 
                                      format, m_texture->getData().extent.x, m_texture->getData().extent.y, GL_TRUE);
    }

    //only update valid data
    void* dataPtr = *((void**)&m_texture->getData().data);
    const bool hasUserData = (dataPtr != nullptr);

    //Detect format
    GLenum internalFormat = format;
    GLenum layout         = getGLTextureLayout(m_texture->getData());
    GLenum type           = m_texture->getData().isHDR ? GL_FLOAT : GL_UNSIGNED_BYTE;

    //Depth formats must NOT use glTextureSubImage2D!
    if (internalFormat == GL_DEPTH_COMPONENT16 ||
        internalFormat == GL_DEPTH_COMPONENT24 ||
        internalFormat == GL_DEPTH_COMPONENT32F)
    {
        //Initialize depth textures to 1.0f
        float depthClear = 1.0f;
        glClearTexImage(m_glTex, 0, GL_DEPTH_COMPONENT, GL_FLOAT, &depthClear);
    }
    else if (internalFormat == GL_DEPTH24_STENCIL8 ||
            internalFormat == GL_DEPTH32F_STENCIL8)
    {
        //Initialize depth-stencil to zeros
        GLuint clear = 0;
        glClearTexImage(m_glTex, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, &clear);
    }
    else
    {
        //Color texture upload
        if (!hasUserData)
        {
            //allocate zeroed data if user did not supply any
            const uint32_t w = m_texture->getData().extent.x;
            const uint32_t h = m_texture->getData().extent.y;

            //Compute pixel size
            uint32_t pixelSize = 1;
            switch (layout)
            {
            case GL_RED:  pixelSize = 1; break;
            case GL_RG:   pixelSize = 2; break;
            case GL_RGB:  pixelSize = 3; break;
            case GL_RGBA: pixelSize = 4; break;
            default:      pixelSize = 4; break;
            }

            if (m_texture->getData().isHDR)
                pixelSize *= sizeof(float);
            else
                pixelSize *= sizeof(uint8_t);

            const size_t dataSize = size_t(w) * size_t(h) * pixelSize;

            void* zero = calloc(1, dataSize);   //zero-initialized

            glTextureSubImage2D(
                m_glTex, 0, 0,0, w, h,
                layout,
                type,
                zero
            );

            free(zero);
        }
        else
        {
            //Upload user data
            glTextureSubImage2D(
                m_glTex, 0, 0,0, 
                m_texture->getData().extent.x, 
                m_texture->getData().extent.y, 
                layout, 
                type, 
                dataPtr
            );
        }
    }

    //for multi sample textures, the setup is done here
    if (m_samples != GLGE_TEXTURE_SAMPLE_X1) {return;}

    //select the correct filter state
    glTextureParameteri(m_glTex, GL_TEXTURE_MIN_FILTER, (m_filterMode == GLGE_FILTER_MODE_LINEAR) ? GL_LINEAR : GL_NEAREST);
    glTextureParameteri(m_glTex, GL_TEXTURE_MAG_FILTER, (m_filterMode == GLGE_FILTER_MODE_LINEAR) ? GL_LINEAR : GL_NEAREST);
    //if supported and requested, enable anisotropic filtering
    if (GLAD_GL_EXT_texture_filter_anisotropic && (m_anisotropy != 0.f)) {
        //get the maximum supported anisotropic value
        float maxAnisotropy = 0.f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
        //set the anisotropy value
        glTextureParameterf(m_glTex, GL_TEXTURE_MAX_ANISOTROPY, (m_anisotropy < maxAnisotropy) ? m_anisotropy : maxAnisotropy);
    }
    //finalize the texture
    glTextureParameteri(m_glTex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_glTex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}