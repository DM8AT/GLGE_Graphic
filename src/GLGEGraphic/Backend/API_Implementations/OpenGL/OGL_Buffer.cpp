/**
 * @file OGL_Buffer.cpp
 * @author DM8AT
 * @brief implement the OpenGL general purpose buffer
 * @version 0.1
 * @date 2025-11-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//add the buffer API
#include "OGL_Buffer.h"
//add memcpy
#include <cstring>
//add OpenGL
#include "glad/glad.h"

static GLenum getUsageHint(GLGE::Graphic::Backend::OGL::Buffer::Type type) noexcept
{
    switch (type)
    {
    case GLGE::Graphic::Backend::OGL::Buffer::Type::SHADER_STORAGE_BUFFER:
        return GL_DYNAMIC_COPY;
        break;
    case GLGE::Graphic::Backend::OGL::Buffer::Type::UNIFORM_BUFFER:
        return GL_DYNAMIC_COPY;
        break;
    case GLGE::Graphic::Backend::OGL::Buffer::Type::VERTEX_BUFFER:
        return GL_STATIC_DRAW;
        break;
    case GLGE::Graphic::Backend::OGL::Buffer::Type::INDEX_BUFFER:
        return GL_STATIC_DRAW;
        break;
    
    default:
        return GL_DYNAMIC_DRAW;
        break;
    }
}

GLGE::Graphic::Backend::OGL::Buffer::~Buffer()
{
    //clean up
    glDeleteBuffers(1, &m_buff);
}

void GLGE::Graphic::Backend::OGL::Buffer::forceCreate()
{
    //create the OpenGL buffer
    glCreateBuffers(1, &m_buff);
}

void GLGE::Graphic::Backend::OGL::Buffer::update() noexcept
{
    //sanity check that the size is not 0
    if (m_size == 0) {return;}
    //thread safety
    std::unique_lock lock(m_dataMtx);

    //check if the OpenGL buffer exists
    if (!m_buff) {
        //if it does not exist, resize the buffer
        glCreateBuffers(1, &m_buff);
        m_currSize = 0;
    }
    //if the size is not correct, resize
    if (m_size != m_currSize) {
        //re-size the data
        glNamedBufferStorage(m_buff, m_size, m_data, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
        //store the new size
        m_currSize = m_size;
    } else 
    {
        //fill the buffer with the new data
        void* data = glMapNamedBuffer(m_buff, GL_WRITE_ONLY);
        //only copy data if the mapping was successful
        if (data) {
            memcpy(data, m_data, m_currSize);
            glUnmapNamedBuffer(m_buff);
        }
    }
}