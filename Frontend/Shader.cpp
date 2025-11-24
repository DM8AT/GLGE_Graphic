/**
 * @file Shader.cpp
 * @author DM8AT
 * @brief implement the frontend shader class
 * @version 0.1
 * @date 2025-11-01
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//add the frontend shader
#include "Shader.h"
//add debugging
#include "../../GLGE_BG/Debugging/Logging/__BG_SimpleDebug.h"

//add the shader backend
#include "../Backend/API_Implementations/AllImplementations.h"
//add the backend instance
#include "../Backend/Instance.h"

Shader::Shader(const ShaderStage* stages, uint64_t stageCount) noexcept
{
    //iterate over all entries in the array and store them correctly
    const ShaderStage* end = stages + stageCount;
    for (auto it = stages; it != end; ++it) {
        //compute the index into the bitmask 
        uint8_t idx = 1 << (uint8_t)it->stage;
        //sanity check to not create multiple elements
        GLGE_DEBUG_ASSERT("Multiple initializations of the same element", (m_used & idx) != 0);
        //store the new element
        m_stages[(uint8_t)it->stage] = *it;
        m_used |= idx;
    }

    //create the actual shader
    switch (GLGE::Graphic::Backend::INSTANCE.getAPI())
    {
    case GLGE_GRAPHIC_INSTANCE_API_OPEN_GL:
        m_shader = new GLGE::Graphic::Backend::OGL::Shader(this);
        break;
    
    default:
        break;
    }
}

Shader::~Shader() {
    //if the shader exists, delete it
    if (m_shader) {
        delete (GLGE::Graphic::Backend::API::Shader*)m_shader;
        m_shader = nullptr;
    }
}

bool glge_Shader_RequiresCompile() {return GLGE::Graphic::Backend::API::Shader::isCompileRequired();}

void glge_Shader_Compile() {GLGE::Graphic::Backend::API::Shader::compileAll();}