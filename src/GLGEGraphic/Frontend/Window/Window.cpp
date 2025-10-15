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

//add SDL3 for window creation
#include "SDL3/SDL.h"

Window::Window(const String& name, const uivec2 size) noexcept
 : m_name(name), m_size(size), LayerBase(LayerType(WINDOW_LIBRARY_NAME.data(), WINDOW_LAYER_NAME.data()), Window::staticEventHandler, this, false)
{
    //use an SDL command to issue the window creation
    GLGE::Graphic::Backend::SDL_Command command(&m_window, &m_windowID,
                                                &m_name, &m_size, 0);
    //queue the command
    GLGE::Graphic::Backend::INSTANCE.queueCommand(&command);

    //command is important - wait for the command to finish
    command.waitTillFinished();

    //register to the instance
    if (!GLGE::Graphic::Backend::INSTANCE.getWindowEventStack().addNewLayer(this)) {
        //fatal error
        GLGE_ABORT("[CRITICAL ERROR] Failed to register the window as an event layer to the base instance");
    }
}

Window::~Window()
{
    //use an SDL command to ddelete the window creation
    GLGE::Graphic::Backend::SDL_Command command(m_window);
    //queue the command
    GLGE::Graphic::Backend::INSTANCE.queueCommand(&command);
    //command is important - wait for the command to finish
    command.waitTillFinished();
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