/**
 * @file Material.cpp
 * @author DM8AT
 * @brief implement the frontend material system
 * @version 0.1
 * @date 2025-11-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//add the frontend material system
#include "Material.h"
//add memcpy
#include <cstring>

Material::Material(Shader* shader, Texture** textures, uint8_t textureCount) noexcept
 : m_shader(shader)
{
    //copy over the texture data
    if (textureCount > GLGE_MAX_MATERIAL_TEXTURE_BINDING) {
        //warning
        std::cerr << "[WARNING] Requested to store " << textureCount << " textures but a material may only bind " << GLGE_MAX_MATERIAL_TEXTURE_BINDING << " textures. Truncating data.\n";
        textureCount = GLGE_MAX_MATERIAL_TEXTURE_BINDING;
    }
    //copy the data over
    memcpy(m_textures, textures, textureCount*sizeof(*textures));
}