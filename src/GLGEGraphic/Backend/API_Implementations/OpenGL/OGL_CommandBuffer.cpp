/**
 * @file OGL_CommandBuffer.cpp
 * @author DM8AT
 * @brief implement an OpenGL command buffer
 * @version 0.1
 * @date 2025-10-19
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//add the command buffer API
#include "OGL_CommandBuffer.h"

//use the namespace
using namespace GLGE::Graphic::Backend::OGL;

void CommandBuffer::play() noexcept
{
    //if the command buffer is not marked as recorded, just stop
    if (!m_recorded) {return;}

    //just iterate over commands and play them back
    for (size_t i = 0; i < m_commands.size(); ++i) {
        m_commands[i].execute();
    }
}

void CommandBuffer::clear() noexcept
{
    //just delete all commands
    for (auto& cmd : m_commands) {
        cmd.destroy();
    }
    //clean up the RAM
    m_commands.clear();
    //this can't be recorded anymore
    m_recorded = false;
}
