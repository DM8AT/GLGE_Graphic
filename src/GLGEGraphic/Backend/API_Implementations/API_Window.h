/**
 * @file API_Window.h
 * @author DM8AT
 * @brief define an abstract class for all window API implementations
 * @version 0.1
 * @date 2025-10-17
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_GRAPHIC_BACKEND_API_IMPL_API_WINDOW_
#define _GLGE_GRAPHIC_BACKEND_API_IMPL_API_WINDOW_

//command buffer are required
#include "API_CommandBuffer.h"

/**
 * @brief define the type of VSync to use
 */
typedef enum e_VSync {
    //vsync is disabled
    GLGE_VSYNC_OFF = 0,
    //vsync is enabled
    GLGE_VSYNC_ON,
    //use adaptive vsync if available
    //defaults to vsync if adaptive vsync is not available
    GLGE_VSYNC_ADAPTIVE
} VSync;

//only available for C++
#if __cplusplus

//windows are defined elsewhere
class Window;

//use the GLGE::Graphic::Backend::API namespace
namespace GLGE::Graphic::Backend::API
{

/**
 * @brief define the base class for all graphic APIs
 */
class Window
{
public:

    /**
     * @brief Construct a new Window
     */
    Window(::Window* window) : m_window(window) {}

    /**
     * @brief Destroy the Window
     */
    virtual ~Window() {}

    /**
     * @brief a function that is called when the size of the window changed
     */
    virtual void onResize() noexcept = 0;

    /**
     * @brief make this the current window
     */
    virtual void makeCurrent() noexcept = 0;

    /**
     * @brief prepare the window for the begining of a frame
     * 
     * @param CommandBuffer* the command buffer to record the beginning to
     */
    virtual void beginFrame(CommandBuffer*) noexcept = 0;

    /**
     * @brief end the frame for the window
     * 
     * @param CommandBuffer* the command buffer to record the ending to
     */
    virtual void endFrame(CommandBuffer*) noexcept = 0;

    /**
     * @brief Set the Vsync state of the window
     * 
     * @warning the change will only take effect after the next begin frame event on the window
     * 
     * @param vsync the new vsync state
     */
    inline void setVsync(VSync vsync) noexcept {m_vsync = vsync;}

    /**
     * @brief Get the Vsync state
     * 
     * @return VSync the current vsync state
     */
    inline VSync getVsync() const noexcept {return m_vsync;}

protected:

    /**
     * @brief store a pointer to the frontend window
     */
    ::Window* m_window = nullptr;
    /**
     * @brief store if the window is set to vsync
     */
    VSync m_vsync = GLGE_VSYNC_OFF;

};

};

#endif

#endif