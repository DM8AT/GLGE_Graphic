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

//add the common stuff
#include "../Frontend/Common.h"

//add textures
//all textures are owned by the instance
#include "../Frontend/Texture.h"

//include the layer system from the core - the instance contains a layer stack
#include "../../GLGE_Core/Layers/Layers.h"
//strings are required - interfacing with the windows
#include "../../GLGE_BG/CBinding/String.h"
//math is just important
#include "../../GLGE_Math/GLGEMath.h"
//add the backend instance API
#include "API_Implementations/API_Instance.h"
//add keys
#include "../Frontend/Keys.h"
//add mice
#include "../Frontend/Mouse.h"

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
namespace GLGE::Graphic::Backend {

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

    /**
     * @brief get the currently selected API
     * 
     * @return const InstanceAPI the selected API
     */
    inline const InstanceAPI getAPI() const noexcept {return m_api;}

    /**
     * @brief set the API if possible
     * 
     * @param api the API to select
     * @return true : successfully selected the API
     * @return false : failed to select the API, the graphic API is allready initialized
     */
    inline bool setAPI(InstanceAPI api) noexcept {
        return (m_inst) ? false : ((int)(m_api = api) + 1);
    }

    /**
     * @brief Get the graphic Instance
     * 
     * @return GLGE::Graphic::Backend::API::Instance* a pointer to the internal API instance
     */
    inline GLGE::Graphic::Backend::API::Instance* getInstance() const noexcept
    {return m_inst;}

    /**
     * @brief send an event through the event stack
     * 
     * @param event a pointer to the event to send
     */
    inline void sendEvent(const Event* event) noexcept {m_eventStack.sendEvent(*event);}

    /**
     * @brief access the event stack of the instance
     * 
     * @return LayerStack& a reference to the event stack of the instance
     */
    inline LayerStack& eventStack() noexcept {return m_eventStack;}

    /**
     * @brief Get the Key Layer of the instance
     * 
     * @return const KeyLayer& a constant reference to the key layer
     */
    inline const KeyLayer& getKeyLayer() const noexcept {return m_keyLayer;}

    /**
     * @brief Get the Mouse Layer of the instance
     * 
     * @return const MouseLayer& a constant reference to the internal mouse layer
     */
    inline const MouseLayer& getMouseLayer() const noexcept {return m_miceLayer;}

    /**
     * @brief Get the Global Mouse of the instance
     * 
     * @return const Mouse& a constant reference to the global mouse
     */
    inline const Mouse& getGlobalMouse() const noexcept {return m_globalMouse;}

    /**
     * @brief Get the Relative Mouse of the instance
     * 
     * @return const Mouse& a constant reference to the relative mouse
     */
    inline const Mouse& getRelativeMouse() const noexcept {return m_relativeMouse;}

    //add windows as a friend class so they can access the stuff here
    friend class ::Window;

protected:

    /**
     * @brief store the currently selected API
     */
    InstanceAPI m_api = GLGE_GRAPHIC_INSTANCE_API_AUTO;
    /**
     * @brief store the abstract graphic instance
     */
    GLGE::Graphic::Backend::API::Instance* m_inst = NULL;

    /**
     * @brief Get the Window Flags for the current API
     * 
     * @warning this sets `m_inst` to 1 if it is NULL so the API can no longer change
     * 
     * @return uint32_t the flags for the current API
     */
    uint32_t getWindowFlags() noexcept;

    /**
     * @brief register the creation of a window
     * 
     * @param window the window that was created
     */
    void registerWindow(::Window* window) noexcept;

    /**
     * @brief register the deletion of a window
     * 
     * @param window the window that was deleted
     */
    void deregisterWindow(::Window* window) noexcept;

    /**
     * @brief if the API is set to auto, this sets the API to a usable value
     */
    void resolveAPI() noexcept;

    /**
     * @brief store the amount of active windows
     */
    uint64_t m_activeWindowCount = 0;

    //store a relative mouse state
    Mouse m_relativeMouse;
    //store a global mouse state
    Mouse m_globalMouse;
    //store a layer to handle mice
    MouseLayer m_miceLayer;
    //store the event layer to handle keys
    KeyLayer m_keyLayer;
    /**
     * @brief store a general stack for event handling
     */
    LayerStack m_eventStack;
    /**
     * @brief store a stack of the window layers so the instance can communicate with the windows
     */
    LayerStack m_windowEventStack;
    
};

//store a global instance
extern Instance INSTANCE;

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