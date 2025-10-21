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

Window::Window(const String& name, const uivec2 size, const WindowSettings& settings) noexcept
 : m_name(name), m_size(size), LayerBase(LayerType(WINDOW_LIBRARY_NAME.data(), "INVALID_NAME"), Window::staticEventHandler, this, false)
{
    //get the window flags
    uint32_t flags = 0;
    //add all API dependant flags
    flags |= GLGE::Graphic::Backend::INSTANCE.getWindowFlags();

    //add all flags related to the window settings
    flags |= settings.fullscreen ? SDL_WINDOW_FULLSCREEN : 0;
    flags |= settings.hidden ? SDL_WINDOW_HIDDEN : 0;
    flags |= settings.borderless ? SDL_WINDOW_BORDERLESS : 0;
    flags |= settings.resizable ? SDL_WINDOW_RESIZABLE : 0;
    flags |= settings.minimized ? SDL_WINDOW_MINIMIZED : 0;
    flags |= settings.maximized ? SDL_WINDOW_MAXIMIZED : 0;
    flags |= settings.mouse_grabbed ? SDL_WINDOW_MOUSE_GRABBED : 0;
    flags |= settings.input_focus ? SDL_WINDOW_INPUT_FOCUS : 0;
    flags |= settings.mouse_focus ? SDL_WINDOW_MOUSE_FOCUS : 0;
    flags |= settings.modal ? SDL_WINDOW_MODAL : 0;
    flags |= settings.high_DPI ? SDL_WINDOW_HIGH_PIXEL_DENSITY : 0;
    flags |= settings.mouse_capture ? SDL_WINDOW_MOUSE_CAPTURE : 0;
    flags |= settings.always_on_top ? SDL_WINDOW_ALWAYS_ON_TOP : 0;
    flags |= settings.utility ? SDL_WINDOW_UTILITY : 0;
    flags |= settings.tooltip ? SDL_WINDOW_TOOLTIP : 0;
    flags |= settings.popup_menu ? SDL_WINDOW_POPUP_MENU : 0;
    flags |= settings.keyboard_grabbed ? SDL_WINDOW_KEYBOARD_GRABBED : 0;
    flags |= settings.transparent ? SDL_WINDOW_TRANSPARENT : 0;
    flags |= settings.can_not_focus ? SDL_WINDOW_NOT_FOCUSABLE : 0;

    //create and sanity check the SDL window
    m_window = SDL_CreateWindow(m_name.c_str(), m_size.x, m_size.y, flags);
    GLGE_ASSERT("Failed to create SDL3 window", !m_window);
    m_windowID = SDL_GetWindowID((SDL_Window*)m_window);
    
    //set the name correctly
    std::string name_str = std::to_string(m_windowID);
    char* t_name = new char[name_str.length() + 1];
    memcpy(t_name, name_str.c_str(), name_str.length() + 1);
    m_type.name = t_name;

    //register to the instance
    GLGE::Graphic::Backend::INSTANCE.registerWindow(this);
    
    //create the API window
    switch (GLGE::Graphic::Backend::INSTANCE.getAPI())
    {
    case GLGE_GRAPHIC_INSTANCE_API_OPEN_GL:
        m_api = new GLGE::Graphic::Backend::OGL::Window(this, settings);
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

    //free the type name (it is dynamically allocated)
    delete[] (char*)m_type.name;
    m_type.name = 0;
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
    else if (*event == Event(WINDOW_EVENT_TYPE_MINIMIZED, EventData(nullptr, 0))) {
        //check if this is the correct window. Only if this is the correct window, continue
        if (*((uint32_t*)event->data.data) != m_windowID) {
            return;
        }
        //update the minimized state
        m_api->m_settings.minimized = true;
        m_api->m_settings_requested.minimized = true;
    }
    else if (*event == Event(WINDOW_EVENT_TYPE_RESTORED, EventData(nullptr, 0))) {
        //check if this is the correct window. Only if this is the correct window, continue
        if (*((uint32_t*)event->data.data) != m_windowID) {
            return;
        }
        //restore the window correctly
        SDL_WindowFlags flags = SDL_GetWindowFlags((SDL_Window*)m_window);
        m_api->m_settings.minimized = (bool)(flags & SDL_WINDOW_MINIMIZED);
        m_api->m_settings_requested.minimized = m_api->m_settings.minimized;
        m_api->m_settings.maximized = (bool)(flags & SDL_WINDOW_MAXIMIZED);
        m_api->m_settings_requested.maximized = m_api->m_settings.maximized;
    }
    else if (*event == Event(WINDOW_EVENT_TYPE_MINIMIZED, EventData(nullptr, 0))) {
        //check if this is the correct window. Only if this is the correct window, continue
        if (*((uint32_t*)event->data.data) != m_windowID) {
            return;
        }
        //update the maximized state
        m_api->m_settings.maximized = true;
        m_api->m_settings_requested.maximized = true;
    }
    else if (*event == Event(WINDOW_EVENT_TYPE_SIZE_CHANGE, EventData(nullptr, 0))) {
        //check if this is the correct window. Only if this is the correct window, continue
        if (*((uint32_t*)event->data.data) != m_windowID) {
            return;
        }
        //send the resizing to the API
        m_api->onResize();
    }
}

//just return the amount of active windows
uint64_t Window::getActiveWindowCount() noexcept
{return GLGE::Graphic::Backend::INSTANCE.m_activeWindowCount;}