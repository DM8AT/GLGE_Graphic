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

//include all backends
#include "API_Implementations/AllImplementations.h"

//locally use the backend namespace
using namespace GLGE::Graphic::Backend;

//implement the instance

Instance::Instance()
 : m_windowEventStack(std::vector<LayerBase*>())
{
    //set the default hints for GLGE
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "0");
    SDL_SetHint(SDL_HINT_MAC_CTRL_CLICK_EMULATE_RIGHT_CLICK, "1");
    SDL_SetHint(SDL_HINT_WINDOW_ACTIVATE_WHEN_RAISED, "1");
    SDL_SetHint(SDL_HINT_MOUSE_TOUCH_EVENTS, "1");
    SDL_SetHint(SDL_HINT_VIDEO_SYNC_WINDOW_OPERATIONS, "0");
    SDL_SetHint(SDL_HINT_VIDEO_WAYLAND_SCALE_TO_DISPLAY, "1");
    SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "1");
    SDL_SetHint(SDL_HINT_VIDEO_SYNC_WINDOW_OPERATIONS, "0");
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
        case SDL_EVENT_WINDOW_MINIMIZED:
            //handle the minimized event
            m_windowEventStack.sendEvent(Event(WINDOW_EVENT_TYPE_MINIMIZED, EventData(&e.window.windowID, sizeof(e.window.windowID))));
            break;
        case SDL_EVENT_WINDOW_RESTORED:
            //handle the window restoring
            m_windowEventStack.sendEvent(Event(WINDOW_EVENT_TYPE_RESTORED, EventData(&e.window.windowID, sizeof(e.window.windowID))));
            break;
        case SDL_EVENT_WINDOW_MAXIMIZED: 
            //handle the window maximizing
            m_windowEventStack.sendEvent(Event(WINDOW_EVENT_TYPE_MAXIMIZED, EventData(&e.window.windowID, sizeof(e.window.windowID))));
            break;
        case SDL_EVENT_WINDOW_RESIZED:
            //handle the window resizing
            m_windowEventStack.sendEvent(Event(WINDOW_EVENT_TYPE_SIZE_CHANGE, EventData(&e.window, sizeof(e.window))));
            break;

        default:
            break;
        }
    }

    //tick the backend instance
    m_inst->tick();
    m_inst->onUpdate();
}

void Instance::registerWindow(Window* window) noexcept
{
    //add the window as an input layer
    if (!m_windowEventStack.addNewLayer(window)) {
        //fatal error
        GLGE_ABORT("[CRITICAL ERROR] Failed to register the window as an event layer to the base instance");
    }

    //check if this was the first window
    if (m_activeWindowCount == 0) {
        //check if a new instance must created (this should be the case)
        GLGE_ASSERT("No window exists, but the graphic API is initialized. This should not be the case. ", 
                    (m_inst != nullptr) && (m_inst != (GLGE::Graphic::Backend::API::Instance*)1));

        //create the new instance
        //first, get rid of the auto api
        resolveAPI();

        //switch over the api and select the correct one
        switch (m_api)
        {
        case GLGE_GRAPHIC_INSTANCE_API_OPEN_GL:
            m_inst = new GLGE::Graphic::Backend::OGL::Instance(window);
            break;
        
        default:
            break;
        }
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

    //check if this was the last window
    if (m_activeWindowCount == 0) {
        //de-register the graphic API
        if (m_inst) {
            delete m_inst;
            m_inst = nullptr;
        }
    }
}

uint32_t Instance::getWindowFlags() noexcept
{
    //if the instance is NULL, set it to 1 to lock the API
    if (!m_inst) {m_inst = (GLGE::Graphic::Backend::API::Instance*)1;}
    //if the API is auto, resolve the API
    resolveAPI();
    //switch over the API
    switch (m_api)
    {
    case GLGE_GRAPHIC_INSTANCE_API_OPEN_GL:
        //return the OpenGL window flags
        return GLGE::Graphic::Backend::OGL::Instance::getWindowFlags();
        break;
    
    default:
        break;
    }
    //just use no flags
    return 0;
}

void Instance::resolveAPI() noexcept
{
    //if the API is not auto, just stop
    if (m_api != GLGE_GRAPHIC_INSTANCE_API_AUTO) {return;}

    //for now, just use OpenGL
    m_api = GLGE_GRAPHIC_INSTANCE_API_OPEN_GL;
}



//initialize the global graphic instance
Instance GLGE::Graphic::Backend::INSTANCE = Instance();


/**
 * @brief store a direct mapping from API enum value to 
 */
static bool GLGE_SUPPORTED_API_MAP[] = {
    true,
    true,
    false,
    false
};

bool glge_graphic_IsAPISupported(InstanceAPI api) {
    //simply return the value from the support map (for now)
    return GLGE_SUPPORTED_API_MAP[(int)api];
}