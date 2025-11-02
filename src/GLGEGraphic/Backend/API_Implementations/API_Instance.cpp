/**
 * @file API_Instance.cpp
 * @author DM8AT
 * @brief implement the tick function that applies the tick to all graphic backend classes
 * @version 0.1
 * @date 2025-10-31
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//add the api instance
#include "API_Instance.h"

//also add textures to update them
#include "API_Texture.h"
//as well ass the buffers
#include "API_Buffer.h"

void GLGE::Graphic::Backend::API::Instance::tick() noexcept
{
    //textures
    {
        //thread safety
        std::unique_lock lock(API::Texture::m_updateMutex);
        //update the textures
        for (Texture* tex : API::Texture::m_toUpdate) {tex->tickGPU();}
        API::Texture::m_toUpdate.clear();
    }

    //buffers
    {
        //thread safety
        std::unique_lock lock(API::Buffer::m_mutex);
        //update the buffer and mark them as not updated
        for (Buffer* buff : API::Buffer::m_queue) {
            buff->update();
            buff->m_queued.store(false, std::memory_order_release);
        }
        API::Buffer::m_queue.clear();
    }
};