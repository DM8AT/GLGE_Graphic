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
 : m_type(type), m_buff(new (m_buffStorage) GLGE::Graphic::Backend::API::CycleBuffer(data, size, 
    (type == GLGE_BUFFER_TYPE_SHADER_STORAGE) ? GLGE::Graphic::Backend::API::Buffer::SHADER_STORAGE_BUFFER : 
        GLGE::Graphic::Backend::API::Buffer::UNIFORM_BUFFER
    , bufferCount))
{
    //sanity check the buffer
    static_assert(sizeof(m_buffStorage) == sizeof(GLGE::Graphic::Backend::API::CycleBuffer), "A cycle buffer is not the same size as the buffer expects");
}

const void* Buffer::getData() const noexcept {
    //return the raw data
    return ((GLGE::Graphic::Backend::API::CycleBuffer*)m_buff)->getRaw();
}

uint64_t Buffer::getSize() const noexcept {
    //return the buffer size
    return ((GLGE::Graphic::Backend::API::CycleBuffer*)m_buff)->getSize();
}

void Buffer::setData(void* data, uint64_t size) noexcept {
    //write the data
    ((GLGE::Graphic::Backend::API::CycleBuffer*)m_buff)->set(data, size);
}

void Buffer::write(void* data, uint64_t offset, uint64_t size) noexcept {
    //write the data
    ((GLGE::Graphic::Backend::API::CycleBuffer*)m_buff)->write(data, size, offset);
}

void Buffer::resize(uint64_t size) noexcept {
    //change the size of the buffer
    ((GLGE::Graphic::Backend::API::CycleBuffer*)m_buff)->resize(size);
}