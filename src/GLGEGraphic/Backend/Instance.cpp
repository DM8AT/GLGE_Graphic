/**
 * @file Instance.cpp
 * @author DM8AT
 * @brief implement the functionality for graphic instances
 * @version 0.1
 * @date 2025-10-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//include graphic instances
#include "Instance.h"
//SDL3 is required
#include "SDL3/SDL.h"
//include the debugging from the background library
#include "../../GLGE_BG/Debugging/Logging/__BG_SimpleDebug.h"

//include the windows for the window event types
#include "../Frontend/Window/Window.h"

//locally use the backend namespace
using namespace GLGE::Graphic::Backend;

//implement the instance

Instance::Instance()
 : m_windowEventStack(std::vector<LayerBase*>())
{
    //initialize SDL3 (this thread is the main thread as this is a static type)
    SDL_InitSubSystem(SDL_INIT_VIDEO);
}

Instance::~Instance()
{
    //stop SDL
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_Quit();
}

void Instance::update() noexcept
{
    //poll all SDL events
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        //check the SDL event
        switch (e.type)
        {
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            //handle the window close request event
            m_windowEventStack.sendEvent(Event(WINDOW_EVENT_TYPE_CLOSE_REQUEST, EventData(&e.window.windowID, sizeof(e.window.windowID))));
            break;
        
        default:
            break;
        }
    }
}

void Instance::registerWindow(Window* window) noexcept
{
    //add the window as an input layer
    if (!m_windowEventStack.addNewLayer(window)) {
        //fatal error
        GLGE_ABORT("[CRITICAL ERROR] Failed to register the window as an event layer to the base instance");
    }
    //increase the amount of known windows
    ++m_activeWindowCount;
}

void Instance::deregisterWindow(Window* window) noexcept
{
    //get the ID of the window
    uint64_t id = m_windowEventStack.getLayerIndex(window->getType().library, window->getType().name);
    //sanity check of the ID
    if (id == UINT64_MAX) {
        //debug message
        GLGE_DEBUG_MESSAGE("Invalid window ID: Window de-registration skipped");
        return;
    }

    //remove the layer
    m_windowEventStack.removeLayer(id);
    //decrease the amount of known windows
    --m_activeWindowCount;
}


//initialize the global graphic instance
Instance GLGE::Graphic::Backend::INSTANCE = Instance();


/**
 * @brief store a direct mapping from API enum value to 
 */
static bool GLGE_SUPPORTED_API_MAP[] = {
    false,
    false,
    false,
    false
};

bool glge_graphic_IsAPISupported(InstanceAPI api) {
    //simply return the value from the support map (for now)
    return GLGE_SUPPORTED_API_MAP[(int)api];
}