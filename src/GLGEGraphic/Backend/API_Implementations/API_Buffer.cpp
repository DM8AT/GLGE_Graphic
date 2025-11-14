/**
 * @file API_Buffer.cpp
 * @author DM8AT
 * @brief 
 * @version 0.1
 * @date 2025-11-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//add the buffer API
#include "API_Buffer.h"
//for memcpy
#include <cstring>
//add debugging
#include "../../../GLGE_BG/Debugging/Logging/__BG_SimpleDebug.h"

GLGE::Graphic::Backend::API::Buffer::Buffer(void* data, uint64_t dataSize, Type type) noexcept
 : m_type(type)
{
    //check if data is requested
    if (data) {
        //make sure this is the only thread that writes to the data
        std::unique_lock lock(m_dataMtx);
        //allocate the internal data buffer
        m_data = new uint8_t[dataSize];
        //sanity check
        GLGE_ASSERT("Failed to allocate the CPU side data buffer of a graphic buffer", !m_data);
        //store the size of the allocated block
        m_size = dataSize;
        //copy the data over
        memcpy(m_data, data, dataSize);
        //queue an update
        queueUpdate();
    }
}

GLGE::Graphic::Backend::API::Buffer::~Buffer() noexcept
{
    //if data exists, delete it
    if (m_data) {
        delete[] (uint8_t*)m_data;
        m_data = nullptr;
    }
}

void GLGE::Graphic::Backend::API::Buffer::resize(uint64_t newSize) noexcept
{
    //make sure this is the only thread that writes to the data
    std::unique_lock lock(m_dataMtx);
    //allocate the new data
    void* newData = new uint8_t[newSize](0);
    GLGE_ASSERT("Failed to allocate the new data for the resizing of a CPU side buffer", !newData);
    //copy the old data over
    memcpy(newData, m_data, (m_size > newSize) ? newSize : m_size);
    m_data = newData;
    //store the new size
    m_size = newSize;
    //queue an update
    queueUpdate();
}

void GLGE::Graphic::Backend::API::Buffer::append(void* data, uint64_t dataSize) noexcept
{
    //make sure this is the only thread that writes to the data
    std::unique_lock lock(m_dataMtx);
    //allocate a buffer that is `dataSize` larger than the current block
    void* newData = new uint8_t[dataSize + m_size];
    GLGE_ASSERT("Failed to expand the region of a CPU side buffer", !newData);
    //copy the old data over
    memcpy(newData, m_data, m_size);
    //copy the new data
    memcpy(((uint8_t*)newData) + m_size, data, dataSize);
    m_data = newData;
    //increase the size
    m_size += dataSize;
    //queue an update
    queueUpdate();
}

void GLGE::Graphic::Backend::API::Buffer::queueUpdate() noexcept {
    //only queue an update if none is queued
    if (!m_queued.load(std::memory_order_acquire)) {
        //lock the mutex to make sure that everything is thread safe
        std::unique_lock lock(m_mutex);
        //push this instance to the queue
        m_queue.push_back(this);
        //store that the buffer is queued
        m_queued.store(true, std::memory_order_relaxed);
    }
}




GLGE::Graphic::Backend::API::BufferChain::~BufferChain() noexcept
{
    //thread safety
    std::unique_lock lock(s_newDeleteMut);
    //remove the element from the vector
    for (size_t i = 0; i < s_bufferChains.size(); ++i) 
    {if (s_bufferChains[i] == this) {s_bufferChains.erase(s_bufferChains.begin() + i);}}
    
    //clean up the buffers
    for (uint8_t i = 0; i < cm_bufferCount; ++i) {
        delete m_buffers[i];
        m_buffers[i] = nullptr;
    }
}

void GLGE::Graphic::Backend::API::BufferChain::endWrite() noexcept
{
    //if the amount of buffers is 1, just stop
    if (cm_bufferCount == 1) {return;}

    //get and advance the index
    uint8_t curr = m_curr.load(std::memory_order_relaxed);
    uint8_t next = (curr + 1) % cm_bufferCount;

    //avoid stepping into the buffer the GPU will consume next
    uint8_t consume = m_consume.load(std::memory_order_acquire);
    if (next == consume)
    {
        --next;
        next = (next == 0xFFu) ? cm_bufferCount-1 : next;
    }

    //store the next index as the current one
    m_curr.store(next, std::memory_order_release);
}

void GLGE::Graphic::Backend::API::BufferChain::advanceGPU() noexcept
{
    //if the amount of buffers is 1, just stop
    if (cm_bufferCount == 1) {return;}
    
    //store the current and next index
    uint8_t consume = m_consume.load(std::memory_order_relaxed);
    uint8_t next = (consume + 1) % cm_bufferCount;

    //prevent GPU from stepping onto the buffer that CPU is *currently* writing
    uint8_t curr = m_curr.load(std::memory_order_acquire);
    if (next == curr)
    {
        --next;
        next = (next == 0xFFu) ? cm_bufferCount-1 : next;
    }

    //store the next index
    m_consume.store(next, std::memory_order_release);
}

void GLGE::Graphic::Backend::API::BufferChain::resizeAll(uint64_t newSize)
{
    //iterate over all buffers and resize them
    for (uint8_t i = 0; i < cm_bufferCount; ++i) {
        m_buffers[i]->resize(newSize);
    }
}