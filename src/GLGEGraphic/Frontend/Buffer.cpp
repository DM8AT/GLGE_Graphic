/**
 * @file Buffer.cpp
 * @author DM8AT
 * @brief implement the frontend buffer class
 * @version 0.1
 * @date 2025-11-11
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//add the buffer API
#include "Buffer.h"

//add instances
#include "../Backend/Instance.h"
//also add all backends
#include "../Backend/API_Implementations/AllImplementations.h"

Buffer::Buffer(void* data, uint64_t size, BufferType type, uint8_t bufferCount) 
 : m_type(type), m_buff(new (m_buffStorage) GLGE::Graphic::Backend::API::BufferChain(bufferCount))
{
    //switch over the instance and select the correct API
    switch (GLGE::Graphic::Backend::INSTANCE.getAPI())
    {
    case GLGE_GRAPHIC_INSTANCE_API_OPEN_GL:
        ((GLGE::Graphic::Backend::API::BufferChain*)m_buff)->create<GLGE::Graphic::Backend::OGL::Buffer>(
            data, size, (type == GLGE_BUFFER_TYPE_SHADER_STORAGE) ? GLGE::Graphic::Backend::API::Buffer::SHADER_STORAGE_BUFFER
                                                                  : GLGE::Graphic::Backend::API::Buffer::UNIFORM_BUFFER);
        break;
    
    default:
        break;
    }
}

const void* Buffer::getData() const noexcept {
    //return the raw data
    return ((GLGE::Graphic::Backend::API::BufferChain*)m_buff)->getCurrent_CPU()->getRaw();
}

uint64_t Buffer::getSize() const noexcept {
    //return the buffer size
    return ((GLGE::Graphic::Backend::API::BufferChain*)m_buff)->getCurrent_CPU()->getSize();
}

void Buffer::setData(void* data, uint64_t size) noexcept {
    //start writing to the buffer
    auto buff = ((GLGE::Graphic::Backend::API::BufferChain*)m_buff)->beginWrite();
    //write the data
    buff->set(data, size);
    //finish the writing
    ((GLGE::Graphic::Backend::API::BufferChain*)m_buff)->endWrite();
}

void Buffer::write(void* data, uint64_t offset, uint64_t size) noexcept {
    //start writing to the buffer
    auto buff = ((GLGE::Graphic::Backend::API::BufferChain*)m_buff)->beginWrite();
    //write the data
    buff->write(data, size, offset);
    //finish the writing
    ((GLGE::Graphic::Backend::API::BufferChain*)m_buff)->endWrite();
}

void Buffer::resize(uint64_t size) noexcept {
    //start writing to the buffer
    auto buff = ((GLGE::Graphic::Backend::API::BufferChain*)m_buff)->beginWrite();
    //change the size of the buffer
    buff->resize(size);
    //finish the writing
    ((GLGE::Graphic::Backend::API::BufferChain*)m_buff)->endWrite();
}