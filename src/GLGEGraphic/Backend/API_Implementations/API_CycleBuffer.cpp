/**
 * @file API_CycleBuffer.cpp
 * @author DM8AT
 * @brief implement the cycle buffer
 * @version 0.1
 * @date 2025-11-15
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//add the cycle buffer API
#include "API_CycleBuffer.h"
//add the full backend
#include "AllImplementations.h"
#include "../Instance.h"

//add the API namespace locally
using namespace GLGE::Graphic::Backend::API;

inline static CycleBufferBackend* createBackend(CycleBuffer* buffer, uint8_t idx) {
    //switch over the API
    switch (GLGE::Graphic::Backend::INSTANCE.getAPI())
    {
    case GLGE_GRAPHIC_INSTANCE_API_OPEN_GL:
        return new GLGE::Graphic::Backend::OGL::CycleBufferBackend(buffer, idx);
        break;
    
    default:
        break;
    }
    return nullptr;
}

CycleBuffer::CycleBuffer(void* data, uint64_t size, Buffer::Type type, uint8_t bufferCount)
 : m_type(type), m_size(size), 
   cm_usedBuffers((bufferCount == 0) ? 1 : ((bufferCount > GLGE_BUFFER_MAX_RING_BUFFER_COUNT) ? GLGE_BUFFER_MAX_RING_BUFFER_COUNT : bufferCount))
{
    //create the buffer
    m_data = calloc(m_size, 1);
    if (data) {memcpy(m_data, data, size);}
    //create all the buffer backends
    for (uint8_t i = 0; i < cm_usedBuffers; ++i) {
        m_backends[i] = createBackend(this, i);
    }
    //store this cycle buffer
    {
        //thread safety
        std::unique_lock lock(s_mtx);
        s_buffers.push_back(this);
    }
}

CycleBuffer::~CycleBuffer()
{
    //remove this buffer
    {
        //thread safety
        std::unique_lock lock(s_mtx);
        for (size_t i = 0; i < s_buffers.size(); ++i) {
            if (s_buffers[i] == this) {
                s_buffers.erase(s_buffers.begin() + i);
                break;
            }
        }
    }
    //clean up the buffers
    for (uint8_t i = 0; i < cm_usedBuffers; ++i) {
        delete m_backends[i];
        m_backends[i] = nullptr;
    }
    //clean up the data
    free(m_data);
    m_data = nullptr;
}

void CycleBuffer::tick() noexcept
{
    //thread safety
    std::unique_lock lock(m_mtx);
    //sync the GPU buffer
    getCurrentGPUBackend<API::CycleBufferBackend>()->syncCPU();
    //advance the GPU index
    m_gpuBuff.store((m_gpuBuff.load(std::memory_order_acquire) + 1) % cm_usedBuffers, std::memory_order_release);
}

void CycleBuffer::set(void* data, uint64_t size) noexcept
{
    //thread safety
    std::unique_lock lock(m_mtx);
    //check if the size changed
    if (size != m_size) {
        //if it did, resize the buffer
        resize(size);
    }
    //then, store the data
    memcpy(m_data, data, m_size);
}

void CycleBuffer::write(void* data, uint64_t size, uint64_t offset) noexcept
{
    //thread safety
    std::unique_lock lock(m_mtx);
    //write at the requested position
    memcpy((uint8_t*)m_data + offset, data, size);
}

void CycleBuffer::resize(uint64_t size) noexcept
{
    //thread safety
    std::unique_lock lock(m_mtx);
    //create the new buffer
    void* newBuff = calloc(size, 1);
    //copy over the maximum of data
    memcpy(newBuff, m_data, (size > m_size) ? m_size : size);
    //swap the buffers
    free(m_data);
    m_data = newBuff;
    //store the new size
    m_size = size;
}