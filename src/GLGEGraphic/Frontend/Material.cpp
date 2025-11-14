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
//add the API implementation
#include "../Backend/API_Implementations/AllImplementations.h"
#include "../Backend/Instance.h"
//add memcpy
#include <cstring>

Material::Material(Shader* shader, Texture** textures, uint8_t textureCount, Buffer** buffers, uint8_t bufferCount, const VertexLayout& layout, 
                   MaterialSettings settings, DepthTestOperator depthOperator) noexcept
 : m_shader(shader), m_layout(layout), m_settings(settings), m_depthOperator(depthOperator)
{
    //copy over the texture data
    if (textureCount > GLGE_MAX_MATERIAL_TEXTURE_BINDING) {
        //warning
        std::cerr << "[WARNING] Requested to store " << textureCount << " textures but a material may only bind " << GLGE_MAX_MATERIAL_TEXTURE_BINDING << " textures. Truncating data.\n";
        textureCount = GLGE_MAX_MATERIAL_TEXTURE_BINDING;
    }
    //copy the data over
    memcpy(m_textures, textures, textureCount*sizeof(*textures));
    //store the amount of used textures
    m_usedTextureCount = textureCount;
    
    //copy over the buffer data
    if (bufferCount > GLGE_MAX_MATERIAL_BUFFER_BINDING) {
        //warning
        std::cerr << "[WARNING] Requested to store " << bufferCount << " buffers but a material may only bind " << GLGE_MAX_MATERIAL_BUFFER_BINDING << " buffers. Truncating data.\n";
        bufferCount = GLGE_MAX_MATERIAL_BUFFER_BINDING;
    }
    //copy the data over
    memcpy(m_buffers, buffers, bufferCount * sizeof(*buffers));
    //store how many buffers are used
    m_usedBuffers = bufferCount;

    //switch over the API to create the correct material type
    switch (GLGE::Graphic::Backend::INSTANCE.getAPI())
    {
    case GLGE_GRAPHIC_INSTANCE_API_OPEN_GL:
        m_material = new GLGE::Graphic::Backend::OGL::Material(this);
        break;
    
    default:
        break;
    }
}

Material::~Material() noexcept
{
    //if the material exits, delete it
    if (m_material) {
        //clean up
        delete (GLGE::Graphic::Backend::OGL::Material*)m_material;
        m_material = nullptr;
    }
}