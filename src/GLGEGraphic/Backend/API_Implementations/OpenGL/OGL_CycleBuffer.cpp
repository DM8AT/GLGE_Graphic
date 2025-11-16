/**
 * @file OGL_CycleBuffer.cpp
 * @author DM8AT
 * @brief implement the cycle buffer for OpenGL
 * @version 0.1
 * @date 2025-11-15
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//add the API
#include "OGL_CycleBuffer.h"
//add memcpy
#include <cstring>
//add OpenGL
#include <glad/glad.h>

//use the OpenGL namespace
using namespace GLGE::Graphic::Backend::OGL;

CycleBufferBackend::CycleBufferBackend(API::CycleBuffer* cycleBuffer, uint8_t idx)
 : API::CycleBufferBackend(cycleBuffer, idx)
{
    //directly queue an update
    queueUpdate();
}

void CycleBufferBackend::syncCPU() noexcept
{
    //if a fence exists, only execute if the fence is signaled
    if (m_sync) {
        //get the result - timeout 0 means non-blocking
        GLenum res = glClientWaitSync((GLsync)m_sync, GL_SYNC_FLUSH_COMMANDS_BIT, 0);
        //check if the sync failed
        if ((res == GL_TIMEOUT_EXPIRED) || (res == GL_WAIT_FAILED)) {
            return;
        }
        //clean up the fence - does nothing
        glDeleteSync((GLsync)m_sync);
        m_sync = nullptr;
    }

    //check if the size is up to date
    if (m_size == m_cBuff->getSize()) {
        //just pass the data
        memcpy(m_mapped, m_cBuff->getRaw(), m_size);
    } else {
        //else, update the data
        update();
    }
}

void CycleBufferBackend::markInUse() noexcept
{
    //delete any previous fence
    if (m_sync) {
        glDeleteSync((GLsync)m_sync);
        m_sync = nullptr;
    }
    //create a new fence to mark when the GPU consumed the data
    m_sync = (void*)glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
}

void CycleBufferBackend::update() noexcept
{
    //first, check if the buffer exists
    if (!m_buff) {
        //if not, create it
        glCreateBuffers(1, &m_buff);
    }
    //#error improve the thread safety "https://chatgpt.com/c/6918c4d8-bd64-832b-b699-d726b233c633"
    //check if the size is up to date
    if (m_size != m_cBuff->getSize()) {
        //store the updated values
        m_size = m_cBuff->getSize();
        //store the flags to apply
        GLenum flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
        //update the size
        glNamedBufferStorage(m_buff, m_size, m_cBuff->getRaw(), flags);
        //map the permanent data
        m_mapped = glMapNamedBufferRange(m_buff, 0, m_size, flags);
    }
    //update is no longer required
    m_update.store(false, std::memory_order_release);
}

void CycleBufferBackend::queueUpdate() noexcept
{
    //if this is allready updating, stop
    if (m_update.load(std::memory_order_acquire)) {return;}
    //thread safety
    std::unique_lock lock(m_updateMtx);
    //if this is allready updating, stop
    if (m_update.load(std::memory_order_acquire)) {return;}
    //add to the queue
    m_updateQueue.push_back(this);
    //say that this is updating
    m_update.store(true, std::memory_order_release);
}