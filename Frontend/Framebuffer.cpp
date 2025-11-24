/**
 * @file Framebuffer.cpp
 * @author DM8AT
 * @brief implement the frontend framebuffer
 * @version 0.1
 * @date 2025-11-21
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//add the frontend framebuffer
#include "Framebuffer.h"
//add the API
#include "../Backend/Instance.h"
#include "../Backend/API_Implementations/AllImplementations.h"
//add debugging
#include "../../GLGE_BG/Debugging/Logging/__BG_SimpleDebug.h"

Framebuffer::Framebuffer(Texture* const* textures, uint8_t textureCount) noexcept 
 : m_texCount(textureCount)
{
    //make sure the size is small enough
    GLGE_DEBUG_ASSERT("Too many textures for a framebuffer: " << textureCount << " textures requested, but the maximum is "
                       << GLGE_FRAMEBUFFER_MAX_TEXTURES, 
                      textureCount > GLGE_FRAMEBUFFER_MAX_TEXTURES);
    //keep track of the different kinds of textures (only for debugging)
    GLGE_IN_DEBUG(
        uint8_t colors = 0;
        uint8_t depth = 0;
        uint8_t stencil = 0;
    )
    //store the framebuffer data and also sanity check the data
    for (uint8_t i = 0; i < textureCount; ++i) {
        //sanity check the texture and check what type it is
        GLGE_IN_DEBUG(
            if (textures[i]->isColor()) {
                //check if enough color slots are free
                if (colors >= GLGE_FRAMEBUFFER_MAX_COLOR_ATTACHMENTS) {
                    //error
                    GLGE_DEBUG_ABORT("Too many color attachments for a framebuffer");
                }
                //add to the color count
                ++colors;
            } else if (textures[i]->isDepthStencil()) {
                //check if enough color slots are free
                if ((depth >= GLGE_FRAMEBUFFER_MAX_DEPTH_ATTACHMENTS) || (stencil >= GLGE_FRAMEBUFFER_MAX_STENCIL_ATTACHMENTS)) {
                    //error
                    GLGE_DEBUG_ABORT("Too many depth or stencil attachments for a framebuffer");
                }
                //add to the depth AND stencil count
                ++depth;
                ++stencil;
            } else if (textures[i]->isDepth()) {
                //check if enough color slots are free
                if (depth >= GLGE_FRAMEBUFFER_MAX_DEPTH_ATTACHMENTS) {
                    //error
                    GLGE_DEBUG_ABORT("Too many depth attachments for a framebuffer");
                }
                //add to the depth count
                ++depth;
            } else if (textures[i]->isStencil()) {
                //check if enough color slots are free
                if (stencil >= GLGE_FRAMEBUFFER_MAX_STENCIL_ATTACHMENTS) {
                    //error
                    GLGE_DEBUG_ABORT("Too many stencil attachments for a framebuffer");
                }
                //add to the stencil count
                ++stencil;
            }
        )

        //add the texture
        m_textures[i] = textures[i];
    }

    //create the backend framebuffer
    switch (GLGE::Graphic::Backend::INSTANCE.getAPI())
    {
    case GLGE_GRAPHIC_INSTANCE_API_OPEN_GL:
        m_api = new GLGE::Graphic::Backend::OGL::Framebuffer(this);
        break;
    
    default:
        GLGE_ABORT("Unknown API");
        break;
    }
}

Framebuffer::~Framebuffer() {
    //clean up the framebuffer
    delete (GLGE::Graphic::Backend::API::Framebuffer*)m_api;
    m_api = nullptr;
}

void Framebuffer::recreate() noexcept {
    //just re-create the backend
    ((GLGE::Graphic::Backend::API::Framebuffer*)m_api)->recreate();
}