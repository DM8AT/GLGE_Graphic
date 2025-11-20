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
//add cycle buffers
#include "../API_CycleBuffer.h"

//add glad for OpenGL
#include "glad/glad.h"

using namespace GLGE::Graphic::Backend::OGL;

GLGE::Graphic::Backend::OGL::Window::Window(::Window* window, const WindowSettings& settings)
 : GLGE::Graphic::Backend::API::Window(window, settings)
{
    //clear the whole window using the default
    glClear(GL_COLOR_BUFFER_BIT);
    //update the window direct
    SDL_GL_SwapWindow(m_window->getSDLWindow());
}

void GLGE::Graphic::Backend::OGL::Window::onResize() noexcept
{
    //queue the viewport update
    m_didResize = true;
}

void GLGE::Graphic::Backend::OGL::Window::makeCurrent() noexcept
{
    //make this the current window
    SDL_GL_MakeCurrent(m_window->getSDLWindow(), (SDL_GLContext)((Instance*)GLGE::Graphic::Backend::INSTANCE.getInstance())->getContext());
}

void GLGE::Graphic::Backend::OGL::Window::beginFrame() noexcept
{
    //make this the current window
    makeCurrent();
    //update the SDL stuff
    update();
    //then, only continue if the window is visible
    if (m_settings.minimized) {return;}

    //if needed, handle the resizing
    if (m_didResize) {
        //update the viewport settings
        glViewport(0,0,m_window->getSize().x,m_window->getSize().y);
        //resize handled
        m_didResize.store(false, std::memory_order_relaxed);
    }
}

void GLGE::Graphic::Backend::OGL::Window::clearWindow(const vec4& clearColor) noexcept
{
    //clear the window
    glClearNamedFramebufferfv(0, GL_COLOR, 0, (float*)&clearColor);
    if (m_window->getDepthClear() != GLGE_NAN_F) {
        glClearNamedFramebufferfi(0, GL_DEPTH_STENCIL, 0, m_window->getDepthClear(), 0);
    }
}

void GLGE::Graphic::Backend::OGL::Window::endFrame() noexcept
{
    //swap the window buffer
    SDL_GL_SwapWindow(m_window->getSDLWindow());
}

void GLGE::Graphic::Backend::OGL::Window::update() noexcept
{
    //check if vsync is out of date
    if (m_vsync != m_vs_request)
    {
        //set the new vsync mode
        if (!SDL_GL_SetSwapInterval((m_vs_request == GLGE_VSYNC_OFF) ? 0 : ((m_vs_request == GLGE_VSYNC_ON) ? 1 : -1))) {
            //adaptive VSync not available, use vsync
            SDL_GL_SetSwapInterval(1);
        }
        //store the new vsync state
        m_vsync = m_vs_request;
    }

    //check all requested window flags to match the current flags
    //update fullscreen
    if (m_settings.fullscreen != m_settings_requested.fullscreen) {
        SDL_SetWindowFullscreen(m_window->getSDLWindow(), m_settings_requested.fullscreen);
        m_settings.fullscreen = m_settings_requested.fullscreen;
    }
    //update hidden
    if (m_settings.hidden != m_settings_requested.hidden) {
        if (m_settings_requested.hidden) {
            SDL_HideWindow(m_window->getSDLWindow());
        } else {
            SDL_ShowWindow(m_window->getSDLWindow());
        }
        m_settings.hidden = m_settings_requested.hidden;
    }
    //update borderless
    if (m_settings.borderless != m_settings_requested.borderless) {
        SDL_SetWindowBordered(m_window->getSDLWindow(), m_settings_requested.borderless);
        m_settings.borderless = m_settings_requested.borderless;
    }
    //update resizable
    if (m_settings.resizable != m_settings_requested.resizable) {
        SDL_SetWindowResizable(m_window->getSDLWindow(), m_settings_requested.resizable);
        m_settings.resizable = m_settings_requested.resizable;
    }

    //update minimized
    if (m_settings.minimized != m_settings_requested.minimized) {
        if (m_settings_requested.minimized) {
            SDL_MinimizeWindow(m_window->getSDLWindow());
        } else {
            SDL_RestoreWindow(m_window->getSDLWindow());

            //if the window was requested to be maximized, apply it after restoring
            if (m_settings_requested.maximized) {
                SDL_MaximizeWindow(m_window->getSDLWindow());
            }
        }

        m_settings.minimized = m_settings_requested.minimized;
    }
    //update maximized
    //only change if not currently minimized (handled in restore above)
    if (m_settings.maximized != m_settings_requested.maximized) {
        if (!m_settings.minimized) {
            if (m_settings_requested.maximized) {
                SDL_MaximizeWindow(m_window->getSDLWindow());
            } else {
                SDL_RestoreWindow(m_window->getSDLWindow());
            }
        }

        m_settings.maximized = m_settings_requested.maximized;
    }

    //update mouse grabbed
    if (m_settings.mouse_grabbed != m_settings_requested.mouse_grabbed) {
        SDL_SetWindowMouseGrab(m_window->getSDLWindow(), m_settings_requested.mouse_grabbed);
        m_settings.mouse_grabbed = m_settings_requested.mouse_grabbed;
    }
    //update modal
    if (m_settings.modal != m_settings_requested.modal) {
        SDL_SetWindowModal(m_window->getSDLWindow(), m_settings_requested.modal);
        m_settings.modal = m_settings_requested.modal;
    }
    //update always on top
    if (m_settings.always_on_top != m_settings_requested.always_on_top) {
        SDL_SetWindowAlwaysOnTop(m_window->getSDLWindow(), m_settings_requested.always_on_top);
        m_settings.always_on_top = m_settings_requested.always_on_top;
    }
    //update keyboard grab
    if (m_settings.keyboard_grabbed != m_settings_requested.keyboard_grabbed) {
        SDL_SetWindowKeyboardGrab(m_window->getSDLWindow(), m_settings_requested.keyboard_grabbed);
        m_settings.keyboard_grabbed = m_settings_requested.keyboard_grabbed;
    }
    //update focusable
    if (m_settings.can_not_focus != m_settings_requested.can_not_focus) {
        SDL_SetWindowFocusable(m_window->getSDLWindow(), !m_settings_requested.can_not_focus);
        m_settings.can_not_focus = m_settings_requested.can_not_focus;
    }
    //update relative mode
    if (m_settings.relative != m_settings_requested.relative) {
        SDL_SetWindowRelativeMouseMode(m_window->getSDLWindow(), m_settings_requested.relative);
        m_settings.relative = m_settings_requested.relative;
    }
}