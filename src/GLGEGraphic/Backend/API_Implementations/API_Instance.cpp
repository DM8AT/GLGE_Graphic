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

void GLGE::Graphic::Backend::API::Instance::tick() noexcept
{
    //thread safety
    std::unique_lock lock(API::Texture::m_updateMutex);
    //update the textures
    for (Texture* tex : API::Texture::m_toUpdate) {tex->tickGPU();}
    API::Texture::m_toUpdate.clear();
};