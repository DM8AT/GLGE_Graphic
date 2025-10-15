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

Window::Window(const String& name, const uivec2 size) noexcept
 : m_name(name), m_size(size), LayerBase(LayerType(WINDOW_LIBRARY_NAME.data(), "INVALID"), Window::staticEventHandler, this, false)
{
    //create and sanity check the SDL window
    m_window = SDL_CreateWindow(m_name.c_str(), m_size.x, m_size.y, 0);
    GLGE_ASSERT("Failed to create SDL3 window", !m_window);
    m_windowID = SDL_GetWindowID((SDL_Window*)m_window);
    
    //set the name correctly
    m_type.name = std::to_string(m_windowID).c_str();

    //register to the instance
    GLGE::Graphic::Backend::INSTANCE.registerWindow(this);
}

Window::~Window()
{
    //remove the window
    GLGE::Graphic::Backend::INSTANCE.deregisterWindow(this);
    //close the SDL window
    SDL_DestroyWindow((SDL_Window*)m_window);
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