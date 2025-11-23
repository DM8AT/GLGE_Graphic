/**
 * @file OGL_Framebuffer.cpp
 * @author DM8AT
 * @brief implement the OpenGL framebuffer
 * @version 0.1
 * @date 2025-11-21
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//get backend access
#define SDL_h_

//add the OpenGL framebuffer
#include "OGL_Framebuffer.h"

//add debugging
#include "../../../../GLGE_BG/Debugging/Logging/__BG_SimpleDebug.h"

//add OpenGL
#include "glad/glad.h"
//add the texture frontend
#include "../../../Frontend/Framebuffer.h"
//add the texture backend
#include "OGL_Texture.h"

//use the Backend namespace
using namespace GLGE::Graphic::Backend;

OGL::Framebuffer::Framebuffer(::Framebuffer* fbo)
 : API::Framebuffer(fbo) {
    //add this for construction
    s_De_InitQueue.emplace_back(this, 0);
}

OGL::Framebuffer::~Framebuffer() {
    //add this for deletion
    s_De_InitQueue.emplace_back(nullptr, m_fbo);
}

void OGL::Framebuffer::create() noexcept {
    //create the OpenGL framebuffer
    glCreateFramebuffers(1, &m_fbo);

    //get the frontend framebuffer
    ::Framebuffer* front = getFrontend();

    //store some meta data for the current state of the framebuffer
    uint8_t colorIndex = 0;
    uint32_t width = 0, height = 0;

    //iterate over all attachments
    for (uint8_t i = 0; i < front->getTextureCount(); i++)
    {
        OGL::Texture* tex = (OGL::Texture*)(front->getTextures()[i])->getBackend();
        if (!tex) continue;

        uint32_t texID = tex->getTexture();
        GLenum target = GL_TEXTURE_2D;

        //Get texture dimensions
        if (i == 0) {
            width  = front->getTextures()[i]->getData().extent.x;
            height = front->getTextures()[i]->getData().extent.y;
        }

        //handle color attachments
        if (tex->isColor())
        {
            //add the texture to the next attachment
            GLenum attachment = GL_COLOR_ATTACHMENT0 + (colorIndex++);
            glNamedFramebufferTexture(m_fbo, attachment, texID, 0);
        }
        //else, it may be a depth / stencil attachment
        else if (tex->isDepthStencil())
        {
            //add the texture to the depth stencil attachment
            glNamedFramebufferTexture(m_fbo, GL_DEPTH_STENCIL_ATTACHMENT, texID, 0);
        }
        //else, check for a depth texture
        else if (tex->isDepth())
        {
            //add the texture to the depth attachment
            glNamedFramebufferTexture(m_fbo, GL_DEPTH_ATTACHMENT, texID, 0);
        }
        //else, check for a stencil texture
        else if (tex->isStencil())
        {
            //add the texture to the stencil attachment
            glNamedFramebufferTexture(m_fbo, GL_STENCIL_ATTACHMENT, texID, 0);
        }
    }

    //disable color for the non-color attachments
    if (colorIndex == 0)
    {
        glNamedFramebufferDrawBuffer(m_fbo, GL_NONE);
        glNamedFramebufferReadBuffer(m_fbo, GL_NONE);
    }
    else
    {
        //store what color attachments are used
        GLenum drawBuffers[GLGE_FRAMEBUFFER_MAX_COLOR_ATTACHMENTS];
        //enable the color attachments
        for (uint8_t i = 0; i < colorIndex; i++)
        {drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;}

        //pass the enabled color attachments to OpenGL
        glNamedFramebufferDrawBuffers(m_fbo, colorIndex, drawBuffers);
    }

    //sanity check the framebuffer
    GLenum status = glCheckNamedFramebufferStatus(m_fbo, GL_FRAMEBUFFER);
    GLGE_DEBUG_ASSERT("GLGE Error: Framebuffer incomplete (" << status << ")\n", status != GL_FRAMEBUFFER_COMPLETE);
}

void OGL::Framebuffer::destroy(uint32_t fbo) noexcept {
    //if the FBO exists, delete it
    if (fbo != 0)
    {glDeleteFramebuffers(1, &fbo);}
}

void OGL::Framebuffer::recreate() noexcept {
    //queue the buffer for re-creation
    s_De_InitQueue.emplace_back(this, 1);
}

void OGL::Framebuffer::recreate_backend() noexcept {
    //first, destroy and directly create the framebuffer
    destroy(m_fbo);
    m_fbo = 0;
    create();
}