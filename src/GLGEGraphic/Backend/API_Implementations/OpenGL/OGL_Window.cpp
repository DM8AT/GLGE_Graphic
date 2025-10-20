/**
 * @file OGL_Window.cpp
 * @author DM8AT
 * @brief implement the OpenGL window
 * @version 0.1
 * @date 2025-10-17
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//add the opengl window
#include "OGL_Window.h"

//add SDL for easy access
#include "SDL3/SDL.h"
//add instances
#include "../../Instance.h"
#include "OGL_Instance.h"
//add the window
#include "../../../Frontend/Window/Window.h"

//add glad for OpenGL
#include "glad/glad.h"

using namespace GLGE::Graphic::Backend::OGL;

GLGE::Graphic::Backend::OGL::Window::Window(::Window* window)
 : GLGE::Graphic::Backend::API::Window(window)
{
    //clear the whole window using the default
    glClear(GL_COLOR_BUFFER_BIT);
    //update the window direct
    SDL_GL_SwapWindow(m_window->getSDLWindow());
}

void GLGE::Graphic::Backend::OGL::Window::onResize() noexcept
{
    //update the viewport settings
    glViewport(0,0,m_window->getSize().x,m_window->getSize().y);
}

void GLGE::Graphic::Backend::OGL::Window::makeCurrent() noexcept
{
    //make this the current window
    SDL_GL_MakeCurrent(m_window->getSDLWindow(), (SDL_GLContext)((Instance*)GLGE::Graphic::Backend::INSTANCE.getInstance())->getContext());
}

void GLGE::Graphic::Backend::OGL::Window::beginFrame(GLGE::Graphic::Backend::API::CommandBuffer* cmdBuff) noexcept
{
    //make the command buffer an OpenGL command buffer
    OGL::CommandBuffer* _cmdBuff = (OGL::CommandBuffer*)cmdBuff;
    //make sure to make this the current window
    _cmdBuff->record<Command_MakeCurrent>(m_window);
    //make sure to clear the frame with the correct clear color
    _cmdBuff->record<Command_Clear>(m_window->getClearColor().r, m_window->getClearColor().g, m_window->getClearColor().b, m_window->getClearColor().a,
                                    0, GL_COLOR, 0);
    //update the window
    _cmdBuff->record<Command_UpdateWindow>(m_window);
}

void GLGE::Graphic::Backend::OGL::Window::endFrame(GLGE::Graphic::Backend::API::CommandBuffer* cmdBuff) noexcept
{
    //make the command buffer an OpenGL command buffer
    OGL::CommandBuffer* _cmdBuff = (OGL::CommandBuffer*)cmdBuff;
    //swap the buffers
    _cmdBuff->record<Command_Swap>(m_window);
}

void GLGE::Graphic::Backend::OGL::Window::update() noexcept
{
    //set the new vsync mode
    if (!SDL_GL_SetSwapInterval((m_vsync == GLGE_VSYNC_OFF) ? 0 : ((m_vsync == GLGE_VSYNC_ON) ? 1 : -1))) {
        //adaptive VSync not available, use vsync
        SDL_GL_SetSwapInterval(1);
    }
}