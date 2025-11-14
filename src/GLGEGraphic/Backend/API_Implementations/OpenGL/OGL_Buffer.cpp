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
    if (m_buff)
    {glDeleteBuffers(1, &m_buff);}
}

void GLGE::Graphic::Backend::OGL::Buffer::set(void* data, uint64_t dataSize) noexcept
{
    //thread safety
    std::unique_lock lock(m_dataMtx);

    //store the new size
    m_size = dataSize;

    //allocate the new data and move over the new data
    if (data && dataSize)
    {
        if (!m_data) { m_data = malloc(dataSize); }
        memcpy(m_data, data, dataSize);
    }
    //queue a data write
    queueUpdate();
}

void GLGE::Graphic::Backend::OGL::Buffer::write(void* data, uint64_t dataSize, uint64_t offset) noexcept
{
    //thread safety
    std::unique_lock lock(m_dataMtx);

    //quick bounds check
    if (!m_data || offset + dataSize > m_size) return;

    //copy over the data
    memcpy((uint8_t*)m_data + offset, data, dataSize);

    //queue a data write
    queueUpdate();
}

void GLGE::Graphic::Backend::OGL::Buffer::forceCreate()
{
    //create the OpenGL buffer
    if (!m_buff)
    {glCreateBuffers(1, &m_buff);}
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
        m_mappedPtr = nullptr;
    }
    //if the size is not correct, resize
    if (m_size != m_currSize) {
        //store the OpenGL flags
        GLenum flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
        //re-size the data
        glNamedBufferStorage(m_buff, m_size, m_data, flags);
        //map the data
        m_mappedPtr = glMapNamedBufferRange(m_buff, 0, m_size, flags);
        //store the new size
        m_currSize = m_size;
    } else 
    {
        //just copy the data over
        memcpy(m_mappedPtr, m_data, m_currSize);
    }
}