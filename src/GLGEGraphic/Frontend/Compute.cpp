/**
 * @file Compute.cpp
 * @author DM8AT
 * @brief implement the compute object
 * @version 0.1
 * @date 2025-11-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//add the compute object
#include "Compute.h"
//add debugging utils
#include "../../GLGE_BG/Debugging/Logging/__BG_SimpleDebug.h"

Compute::Compute(Shader& shader, Buffer* const* buffers, uint8_t bufferCount, Texture* const* textures, uint8_t textureCount)
 : m_shader(&shader)
{
    //sanity check the buffer count
    GLGE_DEBUG_ASSERT("Too many buffers passed to compute object: " << bufferCount << " passed, but the maximum buffer count is " << GLGE_MAX_COMPUTE_SHADER_COMBINED_BUFFERS,
                      bufferCount > GLGE_MAX_COMPUTE_SHADER_COMBINED_BUFFERS);
    //store the buffer count
    m_bufferCount = bufferCount;
    //iterate over all buffers and store them
    //while doing that, make sure the amount of SSBOs and UBOs is not passed
    uint8_t ssboCount = 0;
    uint8_t uboCount = 0;
    for (uint8_t i = 0; i < m_bufferCount; ++i) {
        //only do in debug
        #if GLGE_BG_DEBUG
            //handle special buffers
            if (buffers[i] == GLGE_SKIP_SLOT_SHADER_STORAGE_BUFFER) {
                //increase and sanity check the SSBO count
                ++ssboCount;
                if (ssboCount > GLGE_MAX_COMPUTE_SHADER_STORAGE_BUFFERS) {
                    GLGE_DEBUG_ABORT("Too many shader storage buffers passed to compute object");
                }
            } else if (buffers[i] == GLGE_SKIP_SLOT_UNIFORM_BUFFER) {
                //else, increase and sanity check the UBO count
                ++uboCount;
                if (uboCount > GLGE_MAX_COMPUTE_SHADER_UNIFORM_BUFFERS) {
                    GLGE_DEBUG_ABORT("Too many uniform buffers passed to compute object");
                }
            }
            //sanity checker for buffer type count
            else if (buffers[i]->getType() == GLGE_BUFFER_TYPE_SHADER_STORAGE) {
                //increase and sanity check the SSBO count
                ++ssboCount;
                if (ssboCount > GLGE_MAX_COMPUTE_SHADER_STORAGE_BUFFERS) {
                    GLGE_DEBUG_ABORT("Too many shader storage buffers passed to compute object");
                }
            } else {
                //else, increase and sanity check the UBO count
                ++uboCount;
                if (uboCount > GLGE_MAX_COMPUTE_SHADER_UNIFORM_BUFFERS) {
                    GLGE_DEBUG_ABORT("Too many uniform buffers passed to compute object");
                }
            }
        #endif

        //store the new buffer
        m_buffers[i] = buffers[i];
    }

    //sanity check the texture count
    GLGE_DEBUG_ASSERT("Too many textures passed to compute object: " << textureCount << " passed, but the maximum texture count is " << GLGE_MAX_COMPUTE_SHADER_TEXTURES,
                      textureCount > GLGE_MAX_COMPUTE_SHADER_TEXTURES);
    //store the amount of textures
    m_textureCount = textureCount;
    //store the textures
    for (size_t i = 0; i < m_textureCount; ++i) {
        m_textures[i] = textures[i];
    }
}