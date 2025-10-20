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
//add the window settings
#include "../../Frontend/Window/WindowSettings.h"

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
     * 
     * @param window a pointer to the frontend window
     * @param settings the current settings of the window
     */
    Window(::Window* window, const WindowSettings& settings)
     : m_window(window), m_settings(settings), m_settings_requested(settings)
    {}

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
     * automatically called when playing the render pipeline responsible for the window
     */
    virtual void beginFrame() noexcept = 0;

    /**
     * @brief end the frame for the window
     * 
     * automatically called at the end of the render pipeline responsible for the window
     */
    virtual void endFrame() noexcept = 0;

    /**
     * @brief Set the Vsync state of the window
     * 
     * @warning the change will only take effect after the next begin frame event on the window
     * 
     * @param vsync the new vsync state
     */
    inline void setVsync(VSync vsync) noexcept {m_vs_request = vsync;}

    /**
     * @brief Get the Vsync state
     * 
     * @return VSync the current vsync state
     */
    inline VSync getVsync() const noexcept {return m_vsync;}

    /**
     * @brief Set the Fullscreen state of the window
     * 
     * @warning will update after the next window tick
     * 
     * @param fullscreen the new fullscreen state
     */
    inline void setFullscreen(bool fullscreen) noexcept {m_settings_requested.fullscreen = fullscreen;}

    /**
     * @brief Set the Hidden state of the window
     * 
     * @warning will update after the next window tick
     * 
     * @param hidden say if the window is hidden or shown
     */
    inline void setHidden(bool hidden) noexcept {m_settings_requested.hidden = hidden;}

    /**
     * @brief Set the Borderless state of the window
     * 
     * @warning will update after the next window tick
     * 
     * @param borderless say if the window has decorations (border + 'x' button, usw) or not
     */
    inline void setBorderless(bool borderless) noexcept {m_settings_requested.borderless = borderless;}

    /**
     * @brief Set the Resizable state of the window
     * 
     * @warning will update after the next window tick
     * 
     * @param resizable say if the user can change the window size or not
     */
    inline void setResizable(bool resizable) noexcept {m_settings_requested.resizable = resizable;}

    /**
     * @brief Set the Minimized state of the window
     * 
     * If the window is minimized the pipeline will only execute custom stages as well as the window update
     * 
     * @warning will update after the next window tick
     * 
     * @param minimized say if the window is minimized or not
     */
    inline void setMinimized(bool minimized) noexcept {m_settings_requested.minimized = minimized;}

    /**
     * @brief Set the Maximized state of the window
     * 
     * @warning will update after the next window tick
     * 
     * @param maximized say if the window is maximized or not
     */
    inline void setMaximized(bool maximized) noexcept {m_settings_requested.maximized = maximized;}

    /**
     * @brief Set the Mouse Grabbed state of the window
     * 
     * @warning will update after the next window tick
     * 
     * @param hidden say if the window is has the mouse grabbed or not
     */
    inline void setMouseGrabbed(bool mouseGrabbed) noexcept {m_settings_requested.mouse_grabbed = mouseGrabbed;}

    /**
     * @brief Set the Modal state of the window
     * 
     * @warning will update after the next window tick
     * 
     * @param modal say if the window is a modal window or not
     */
    inline void setModal(bool modal) noexcept {m_settings_requested.modal = modal;}

    /**
     * @brief Set the always on top state of the window
     * 
     * @warning will update after the next window tick
     * 
     * @param alwaysOnTop say if the window is always on top or not
     */
    inline void setAlwaysOnTop(bool alwaysOnTop) noexcept {m_settings_requested.always_on_top = alwaysOnTop;}

    /**
     * @brief Set the Keyboard grabbed state of the window
     * 
     * @warning will update after the next window tick
     * 
     * @param keyboardGrabbed say if the window has the keyboard input grabbed or not
     */
    inline void setKeyboardGrabbed(bool keyboardGrabbed) noexcept {m_settings_requested.keyboard_grabbed = keyboardGrabbed;}

    /**
     * @brief Set the Focusable state of the window
     * 
     * @warning will update after the next window tick
     * 
     * @param notFocusable say if the window can not be focused or if it can
     */
    inline void setNotFocusable(bool notFocusable) noexcept {m_settings_requested.can_not_focus = notFocusable;}

    /**
     * @brief Get the current state of the window settings
     * 
     * @return const WindowSettings& the current window setting state
     */
    inline const WindowSettings& getSettings() const noexcept {return m_settings;}

protected:

    //add the actual window as a friend class
    friend class ::Window;

    /**
     * @brief store a pointer to the frontend window
     */
    ::Window* m_window = nullptr;
    /**
     * @brief store if the window is set to vsync
     */
    VSync m_vsync = GLGE_VSYNC_OFF;
    /**
     * @brief store the requested vsync state
     */
    VSync m_vs_request = GLGE_VSYNC_OFF;

    /**
     * @brief store the current state of the window settings
     */
    WindowSettings m_settings;
    /**
     * @brief store the requested settings
     */
    WindowSettings m_settings_requested;

};

};

#endif

#endif