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
     * @brief main update for the main instance
     */
    void update() noexcept;

    /**
     * @brief Get access to the window event stack of the instance
     * 
     * @return LayerStack& a reference to the window event stack
     */
    inline LayerStack& getWindowEventStack() noexcept
    {return m_windowEventStack;}

    //add windows as a friend class so they can access the stuff here
    friend class ::Window;

protected:

    /**
     * @brief register the creation of a window
     * 
     * @param window the window that was created
     */
    void registerWindow(Window* window) noexcept;

    /**
     * @brief register the deletion of a window
     * 
     * @param window the window that was deleted
     */
    void deregisterWindow(Window* window) noexcept;

    /**
     * @brief store the amount of active windows
     */
    uint64_t m_activeWindowCount = 0;

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