/**
 * @file Window.cpp
 * @author DM8AT
 * @brief implement the window stuff
 * @version 0.1
 * @date 2025-10-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//include the window API
#include "Window.h"

//include the backend
#include "../../Backend/Backend.h"
//add SDL3
#include "SDL3/SDL.h"
//include the default window icon
#include "GLGEWindowIcon.h"

//add all api implementations
#include "../../Backend/API_Implementations/AllImplementations.h"

Window::Window(const String& name, const uivec2 size) noexcept
 : m_name(name), m_size(size), LayerBase(LayerType(WINDOW_LIBRARY_NAME.data(), "INVALID"), Window::staticEventHandler, this, false)
{
    //get the window flags
    uint32_t flags = 0;
    //add all API dependant flags
    flags |= GLGE::Graphic::Backend::INSTANCE.getWindowFlags();
    //create and sanity check the SDL window
    m_window = SDL_CreateWindow(m_name.c_str(), m_size.x, m_size.y, flags);
    GLGE_ASSERT("Failed to create SDL3 window", !m_window);
    m_windowID = SDL_GetWindowID((SDL_Window*)m_window);
    
    //set the name correctly
    m_type.name = std::to_string(m_windowID).c_str();

    //register to the instance
    GLGE::Graphic::Backend::INSTANCE.registerWindow(this);
    
    //create the API window
    switch (GLGE::Graphic::Backend::INSTANCE.getAPI())
    {
    case GLGE_GRAPHIC_INSTANCE_API_OPEN_GL:
        m_api = new GLGE::Graphic::Backend::OGL::Window(this);
        break;
    
    default:
        break;
    }

    //set the default icon for the window
    SDL_Surface* icon = SDL_CreateSurfaceFrom(gimp_image.width, gimp_image.height, SDL_PIXELFORMAT_ARGB8888, 
                                              (void*)gimp_image.pixel_data, gimp_image.width*gimp_image.bytes_per_pixel);
    SDL_SetWindowIcon((SDL_Window*)m_window, icon);
    SDL_DestroySurface(icon);
}

Window::~Window()
{
    //delete the API dependent window
    delete ((GLGE::Graphic::Backend::API::Window*)m_api);
    //remove the window
    GLGE::Graphic::Backend::INSTANCE.deregisterWindow(this);
    //close the SDL window
    SDL_DestroyWindow((SDL_Window*)m_window);
}

void Window::rename(const String& name) noexcept {
    //store the new name
    m_name = name;
    //chang the name using SDL3
    SDL_SetWindowTitle((SDL_Window*)m_window, m_name.c_str());
}

void Window::handleEvent(const Event* event) 
{
    //check if the event is for this layer
    if (*event == Event(WINDOW_EVENT_TYPE_CLOSE_REQUEST, EventData((void*)0, 0))) {
        //check if this is the correct window. Only if this is the correct window, say that closing is requested
        if (*((uint32_t*)event->data.data) == m_windowID) {
            m_requestedClosing = true;
        }
    }
}

//just return the amount of active windows
uint64_t Window::getActiveWindowCount() noexcept
{return GLGE::Graphic::Backend::INSTANCE.m_activeWindowCount;}