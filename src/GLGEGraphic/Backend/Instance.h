/**
 * @file Instance.h
 * @author DM8AT
 * @brief define what a graphic instance is
 * @warning only available for C++
 * @version 0.1
 * @date 2025-10-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_CORE_BACKEND_INSTANCE_
#define _GLGE_CORE_BACKEND_INSTANCE_

//for C add booleans
#include <stdbool.h>
//add the default types
#include "../../GLGE_Core/Types.h"

//define a C/C++ enum for the type of Graphic API
typedef enum e_InstanceAPI {
    //automatically select the best API available
    GLGE_GRAPHIC_INSTANCE_API_AUTO = 0,
    //use OpenGL (uses OpenGL 4.6)
    GLGE_GRAPHIC_INSTANCE_API_OPEN_GL,
    //use Vulkan
    GLGE_GRAPHIC_INSTANCE_API_VULKAN,
    //use directX 11
    GLGE_GRAPHIC_INSTANCE_API_DIRECT_X11,
    //use directX 12
    GLGE_GRAPHIC_INSTANCE_API_DIRECT_X12
} InstanceAPI;

//include the layer system from the core - the instance contains a layer stack
#include "../../GLGE_Core/Layers/Layers.h"
//strings are required - interfacing with the windows
#include "../../GLGE_BG/CBinding/String.h"
//math is just important
#include "../../GLGE_Math/GLGEMath.h"

//only allowed for C++
#if __cplusplus

//add threads - the instance has its own SDL thread
#include <thread>
//add queues - a queue is used to schedule SDL3 commands
#include <queue>

//add a simple mutex - needed for thread synchronization
#include <mutex>
//include conditional variables for thread synchronization
#include <condition_variable>

//windows will be define elsewhere
class Window;

//use a custom namespace for the backend: GLGE::Graphic::Backend
namespace GLGE {
namespace Graphic {
namespace Backend {

/**
 * @brief a structure to store a single SDL3 command
 */
struct SDL_Command {
    //define what type of command this is
    enum CommandType {
        //empty test command
        NOOP = 0,
        //create an SDL window
        CREATE_WINDOW,
        //delete an SDL window
        DELETE_WINDOW
    } command;

    //store the data for the command
    union Data {
        //store data for a window creation
        struct WindowCreate {
            //store the position to store the window in
            void** window;
            //store a pointer to the window id
            uint32_t* id;
            //store the name of the window
            const String* name;
            //store the size of the window
            const uivec2* size;
            //store the flags for the window
            uint32_t flags;
        } windowCreate;
        //store data for a window deletion
        struct WindowDelete {
            //store the window to delete
            void* window;
        } windowDelete;
    } data;

    //store if the command is finished
    bool finished = false; 

    //wait till the execution is finished
    void waitTillFinished() noexcept {
        //lock the mutex to block this thread
        std::unique_lock<std::mutex> lock(mtx);
        //wait till the finished variable is set to true to continue the execution
        cv.wait(lock, [this]{ return finished; });
    }

    /**
     * @brief Set the Finished boolean to true to signal that the command is finished
     */
    void setFinished() noexcept {
        //set finished to true in a thread-safe way
        {
            //lock the mutex (sync with the other thread)
            std::lock_guard<std::mutex> lock(mtx);
            //simply update the boolean
            finished = true;
        }
        //notify the other thread that the finished variable updated
        cv.notify_all();
    }

    //Constructor for NOOP
    SDL_Command(CommandType type) : command(type) {
        //No data to initialize
    }

    //Constructor for CREATE_WINDOW
    SDL_Command(void** windowOut, uint32_t* id, const String* name, const uivec2* size, uint32_t flags)
        : command(CREATE_WINDOW), data{
            .windowCreate{
                .window = windowOut,
                .id = id,
                .name = name,
                .size = size,
                .flags = flags
            }
        }
    {}

    //Constructor for DELETE_WINDOW
    SDL_Command(void* window)
        : command(DELETE_WINDOW), data{
            .windowDelete{
                .window = window
            }
        }
    {}

private:
    //used for synchronization
    std::mutex mtx;
    //used to synch the finished variable without blocking a thread fully
    std::condition_variable cv;
};
    
/**
 * @brief define what a graphic instance is
 */
class Instance {
public:

    /**
     * @brief Construct a new Instance
     */
    Instance();

    /**
     * @brief Destroy the Instance
     */
    ~Instance();

    /**
     * @brief get if the SDL thread is running
     * 
     * The SDL thread should be running during the whole application
     * 
     * @return true : the SDL thread is running
     * @return false : the SDL thread is not running
     */
    inline bool isSDLActive() const noexcept {return m_runSDLThread;}

    /**
     * @brief add a new command for SDL
     * 
     * @param command a pointer to the command to queue
     */
    inline void queueCommand(SDL_Command* command) noexcept 
    {m_commands.push(command);}

    /**
     * @brief Get the Queue of commands
     * 
     * @return std::queue<SDL_Command*>& a reference to the command queue
     */
    inline std::queue<SDL_Command*>& getQueue() noexcept 
    {return m_commands;}

    /**
     * @brief Get access to the window event stack of the instance
     * 
     * @return LayerStack& a reference to the window event stack
     */
    inline LayerStack& getWindowEventStack() noexcept
    {return m_windowEventStack;}

protected:

    /**
     * @brief store the SDL3 main thread
     * 
     * Because the application is heavily multi-threaded it is good to have a fixed
     * thread for SDL as SDL is for the most part not thread-safe
     */
    std::thread m_sdlMainThread;
    /**
     * @brief a signal boolean to signal if the SDL thread should run
     */
    bool m_runSDLThread = true;

    /**
     * @brief store a queue for the SDL commands to execute
     */
    std::queue<SDL_Command*> m_commands;

    /**
     * @brief store a stack of the window layers so the instance can communicate with the windows
     */
    LayerStack m_windowEventStack;
    
};

//store a global instance
extern Instance INSTANCE;

}
}
}

#endif

#if __cplusplus
extern "C" {
#endif

/**
 * @brief check if a specific graphic API is supported
 * 
 * If `GLGE_GRAPHIC_INSTANCE_API_AUTO` fails this means that no API is supported
 * 
 * @param api the api to check
 * @return true : the API is not supported
 * @return false : the API is supported
 */
bool glge_graphic_IsAPISupported(InstanceAPI api);

#if __cplusplus
}
#endif

#endif