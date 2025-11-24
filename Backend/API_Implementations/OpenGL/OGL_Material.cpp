/**
 * @file OGL_Material.cpp
 * @author DM8AT
 * @brief implement the material for OpenGL
 * @version 0.1
 * @date 2025-11-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//add the API
#include "OGL_Material.h"
//add OpenGL command buffer
#include "OGL_CommandBuffer.h"

//add OpenGL
#include "glad/glad.h"

void GLGE::Graphic::Backend::OGL::Material::bind(API::CommandBuffer* cmdBuff) noexcept
{
    //add the command
    ((OGL::CommandBuffer*)cmdBuff)->record<Command_BindMaterial>(this);
}

GLGE::Graphic::Backend::OGL::Material::~Material() noexcept {
    if (m_vao) {glDeleteVertexArrays(1, &m_vao);}
}