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
        return GL_R8UI;
        break;
    case GLGE_TEXTURE_R_F:
        return GL_R32F;
        break;
    case GLGE_TEXTURE_R_H:
        return GL_R16F;
        break;
    case GLGE_TEXTURE_RG:
        return GL_RG8UI;
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

GLGE::Graphic::Backend::OGL::Texture::Texture(::Texture* tex)
 : API::Texture(tex) 
{
    //directly mark the texture data as dirty
    markDirty();
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
    } else if (m_dirtFlags.load(std::memory_order_acquire) & FLAG_RESIZE) {
        resize();
    }

    //reset the flags
    m_dirtFlags.store(0, std::memory_order_relaxed);
}

void GLGE::Graphic::Backend::OGL::Texture::recreate() noexcept
{
    //check if the texture exists
    if (m_glTex)
    {
        //if it does, check if it needs to be re-created
        uint32_t width = 0;
        uint32_t height = 0;
        glGetTextureParameterIuiv(m_glTex, GL_TEXTURE_WIDTH, &width);
        glGetTextureParameterIuiv(m_glTex, GL_TEXTURE_HEIGHT, &width);
        if ((width == m_texture->getData().extent.x) && (height == m_texture->getData().extent.y))
        {
            //can re-upload just the data, no re-creation needed
            glTextureSubImage2D(m_glTex, 0, 0,0, m_texture->getData().extent.x, m_texture->getData().extent.y, getGLTextureLayout(m_texture->getData()), 
                                m_texture->getData().isHDR ? GL_FLOAT : GL_UNSIGNED_INT, *((void**)&m_texture->getData().data));
        }
        else
        {
            //data re-creation needed
            //first, clean up
            glDeleteTextures(1, &m_glTex);
        }   
    }

    //create the texture
    glCreateTextures(GL_TEXTURE_2D, 1, &m_glTex);
    GLenum format = getGLTextureFormat(m_texture->getType());
    glTextureStorage2D(m_glTex, 1, format, m_texture->getData().extent.x, m_texture->getData().extent.y);
    //only update valid data
    if (*((void**)&m_texture->getData().data))
    {
        GLenum glTextureLayout = getGLTextureLayout(m_texture->getData());
        GLenum type = m_texture->getData().isHDR ? GL_FLOAT : GL_UNSIGNED_INT;
        glTextureSubImage2D(m_glTex, 0, 0,0, m_texture->getData().extent.x, m_texture->getData().extent.y, glTextureLayout, 
                            type, *((void**)&m_texture->getData().data));
    }
}

void GLGE::Graphic::Backend::OGL::Texture::resize() noexcept
{
    //resize the texture
    glTextureStorage2D(m_glTex, 1, getGLTextureFormat(m_texture->getType()), m_texture->getData().extent.x, m_texture->getData().extent.y);
    //only update valid data
    if (*((void**)&m_texture->getData().data))
    {
        glTextureSubImage2D(m_glTex, 0, 0,0, m_texture->getData().extent.x, m_texture->getData().extent.y, getGLTextureLayout(m_texture->getData()), 
                            m_texture->getData().isHDR ? GL_FLOAT : GL_UNSIGNED_BYTE, *((void**)&m_texture->getData().data));
    }
}