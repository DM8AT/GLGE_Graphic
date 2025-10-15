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

/**
 * @brief this function runs the SDL main thread
 */
static void SDL_MAIN_THREAD();

Instance::Instance()
 : m_runSDLThread(true), m_windowEventStack(std::vector<LayerBase*>())
{
    //create the SDL3 thread
    m_sdlMainThread = std::thread(SDL_MAIN_THREAD);
}
Instance::~Instance() {
    //stop and join the SDL main thread
    m_runSDLThread = false;
    m_sdlMainThread.join();
}

//initialize the global graphic instance
Instance GLGE::Graphic::Backend::INSTANCE = Instance();

/**
 * @brief handle the create window command
 * 
 * @param cmd a pointer to the command to handle
 */
static void SDL_HANDLE_COMMAND_CREATE_WINDOW(SDL_Command* cmd) {
    //create the window using SDL3
    *cmd->data.windowCreate.window = SDL_CreateWindow(cmd->data.windowCreate.name->c_str(), 
                                                      cmd->data.windowCreate.size->x,
                                                      cmd->data.windowCreate.size->y,
                                                      cmd->data.windowCreate.flags);
    //get the window ID
    *(cmd->data.windowCreate.id) = SDL_GetWindowID((SDL_Window*)(*cmd->data.windowCreate.window));
}

/**
 * @brief handle the delete window command
 * 
 * @param cmd a pointer to the command to handle
 */
static void SDL_HANDLE_COMMAND_DELETE_WINDOW(SDL_Command* cmd) {
    //delete the window
    SDL_DestroyWindow((SDL_Window*)cmd->data.windowDelete.window);
}

/**
 * @brief a function that is called from the SDL main thread to handle the commands
 */
static void SDL_HANDLE_COMMANDS() {
    //iterate over all commands
    while (GLGE::Graphic::Backend::INSTANCE.getQueue().size()) {
        //get the front command
        SDL_Command* cmd = GLGE::Graphic::Backend::INSTANCE.getQueue().front();
        GLGE::Graphic::Backend::INSTANCE.getQueue().pop();

        //handle the command according to the type
        switch (cmd->command)
        {
        //handle the window creation
        case GLGE::Graphic::Backend::SDL_Command::CREATE_WINDOW:
            SDL_HANDLE_COMMAND_CREATE_WINDOW(cmd);
            break;

        //handle the window deletion
        case GLGE::Graphic::Backend::SDL_Command::DELETE_WINDOW:
            SDL_HANDLE_COMMAND_DELETE_WINDOW(cmd);
            break;
        
        default:
            break;
        }

        //signal that the command is handled
        cmd->setFinished();
    }
}

/**
 * @brief this function runs the SDL main thread
 */
static void SDL_MAIN_THREAD() {

    //initialize SDL3
    GLGE_ASSERT("[CRITICAL ERROR] Failed to initialize SDL3", !SDL_Init(SDL_INIT_VIDEO));

    //loop while the app is running
    while (GLGE::Graphic::Backend::INSTANCE.isSDLActive())
    {
        //handle the main threads
        SDL_HANDLE_COMMANDS();

        //poll all SDL3 events
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            //switch over the correct event types to select the correct one to parse trough the internal stack
            switch (e.type)
            {
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                //send the closing request to all windows. Also parse the window id
                INSTANCE.getWindowEventStack().sendEvent(Event(WINDOW_EVENT_TYPE_CLOSE_REQUEST, EventData(&e.window.windowID, sizeof(e.window.windowID))));
                break;
            
            default:
                break;
            }
        }
    }

    //stop SDL3
    SDL_Quit();

}


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