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

void GLGE::Graphic::Backend::API::Buffer::set(void* data, uint64_t dataSize) noexcept
{
    //make sure this is the only thread that writes to the data
    std::unique_lock lock(m_dataMtx);
    //check if the data is the same size
    if (dataSize != m_size) {
        //if not (like in most cases) create a new data storage
        delete[] (uint8_t*)m_data;
        m_data = new uint8_t[dataSize];
        //sanity check
        GLGE_ASSERT("Failed to allocate a new CPU side block for a graphic buffer", !m_data);
        //store the size of the allocated block
        m_size = dataSize;
    }
    //copy the data over
    memcpy(m_data, data, dataSize);
    //queue the buffer for an update
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