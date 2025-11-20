/**
 * @file Window.h
 * @author DM8AT
 * @brief define what a window is
 * @version 0.1
 * @date 2025-10-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_GRAPHIC_FRONTEND_WINDOW_WINDOW_
#define _GLGE_GRAPHIC_FRONTEND_WINDOW_WINDOW_

//add strings for window names
#include "../../../GLGE_BG/CBinding/String.h"
//add all math types
#include "../../../GLGE_Math/GLGEMath.h"
//add the layer system
#include "../../../GLGE_Core/Layers/Layers.h"

//add the window settings
#include "WindowSettings.h"

//add the window API
#include "../../Backend/API_Implementations/API_Window.h"

//for C++ add a class
#if __cplusplus

//include string views for utilities
#include <string_view>
//store the library for the window
constexpr std::string_view WINDOW_LIBRARY_NAME = "GLGE_GRAPHIC_WINDOW";

//define the identifiers for the different events for a window
constexpr EventType WINDOW_EVENT_TYPE_CLOSE_REQUEST(WINDOW_LIBRARY_NAME.data(), (1ul << 48ul | 1ul));
constexpr EventType WINDOW_EVENT_TYPE_MINIMIZED(WINDOW_LIBRARY_NAME.data(), (1ul << 48ul | 2ul));
constexpr EventType WINDOW_EVENT_TYPE_RESTORED(WINDOW_LIBRARY_NAME.data(), (1ul << 48 | 3ul));
constexpr EventType WINDOW_EVENT_TYPE_MAXIMIZED(WINDOW_LIBRARY_NAME.data(), (1ul << 48 | 4ul));
constexpr EventType WINDOW_EVENT_TYPE_SIZE_CHANGE(WINDOW_LIBRARY_NAME.data(), (1ul << 48 | 5ul));

/**
 * @brief store what a simple window is
 */
class Window : public LayerBase {
public:

    /**
     * @brief Construct a new Window
     * 
     * @param name the name of the window
     * @param size the size of the window in pixels
     * @param settings the default settings for the window
     */
    Window(const String& name, const uivec2 size, const WindowSettings& settings = GLGE_WINDOW_SETTINGS_DEFAULT) noexcept;

    /**
     * @brief Destroy the Window
     */
    ~Window();

    /**
     * @brief say that closing is requested
     */
    inline void requestClosing() noexcept {m_requestedClosing = true;}

    /**
     * @brief get if closing was requested
     * 
     * @return true : closing was requested
     * @return false : closing is not requested
     */
    inline bool isClosingRequested() const noexcept {return m_requestedClosing;}

    /**
     * @brief change the name of the window
     * 
     * @param name the new name of the window
     */
    void rename(const String& name) noexcept;

    /**
     * @brief Get the Name of the window
     * 
     * @return const String& the name of the window
     */
    inline const String& getName() const noexcept {return m_name;}

    /**
     * @brief Get the Size of the window
     * 
     * @return const uivec2& the size of the window in pixels
     */
    inline const uivec2& getSize() const noexcept {return m_size;}

    /**
     * @brief Set the Clear Color for the window
     * 
     * @param clearColor the clear color in RGBA for the window
     */
    inline void setClearColor(const vec4& clearColor) noexcept {m_clearColor = clearColor;}

    /**
     * @brief Get the Clear Color of the window
     * 
     * @return const vec4& a constant reference to the clear color
     */
    inline const vec4& getClearColor() const noexcept {return m_clearColor;}

    /**
     * @brief Set the depth to clear the depth buffer with
     * 
     * @param clear the depth value to clear the depth buffer with (NAN disables clearing)
     */
    inline void setDepthClear(float clear) noexcept {m_depthClear = clear;}

    /**
     * @brief Get the depth to clear the depth buffer with
     * 
     * @return float the depth to clear the depth buffer with (NAN disables clearing)
     */
    inline float getDepthClear() const noexcept {return m_depthClear;}

    /**
     * @brief Get the amount of currently known active windows
     * 
     * @return uint64_t the amount of registered active windows
     */
    static uint64_t getActiveWindowCount() noexcept;

    /**
     * @brief set the type of vsync to use for the window
     * 
     * @warning the change will only take effect after the next window update
     * 
     * @param vsync the new vsync state to use
     */
    inline void setVSync(VSync vsync) noexcept {m_api->setVsync(vsync);}

    /**
     * @brief get the type of VSync currently used for the window
     * 
     * @return VSync the currently used VSync mode
     */
    inline VSync getVSync() const noexcept {return m_api->getVsync();}

    /**
     * @brief Set the Fullscreen state of the window
     * 
     * @warning will update after the next window tick
     * 
     * @param fullscreen the new fullscreen state
     */
    inline void setFullscreen(bool fullscreen) noexcept {m_api->setFullscreen(fullscreen);}

    /**
     * @brief Set the Hidden state of the window
     * 
     * @warning will update after the next window tick
     * 
     * @param hidden say if the window is hidden or shown
     */
    inline void setHidden(bool hidden) noexcept {m_api->setHidden(hidden);}

    /**
     * @brief Set the Borderless state of the window
     * 
     * @warning will update after the next window tick
     * 
     * @param borderless say if the window has decorations (border + 'x' button, usw) or not
     */
    inline void setBorderless(bool borderless) noexcept {m_api->setBorderless(borderless);}

    /**
     * @brief Set the Resizable state of the window
     * 
     * @warning will update after the next window tick
     * 
     * @param resizable say if the user can change the window size or not
     */
    inline void setResizable(bool resizable) noexcept {m_api->setResizable(resizable);}

    /**
     * @brief Set the Minimized state of the window
     * 
     * If the window is minimized the pipeline will only execute custom stages as well as the window update
     * 
     * @warning will update after the next window tick
     * 
     * @param minimized say if the window is minimized or not
     */
    inline void setMinimized(bool minimized) noexcept {m_api->setMinimized(minimized);}

    /**
     * @brief Set the Maximized state of the window
     * 
     * @warning will update after the next window tick
     * 
     * @param maximized say if the window is maximized or not
     */
    inline void setMaximized(bool maximized) noexcept {m_api->setMaximized(maximized);}

    /**
     * @brief Set the Mouse Grabbed state of the window
     * 
     * @warning will update after the next window tick
     * 
     * @param hidden say if the window is has the mouse grabbed or not
     */
    inline void setMouseGrabbed(bool mouseGrabbed) noexcept {m_api->setMouseGrabbed(mouseGrabbed);}

    /**
     * @brief Set the Modal state of the window
     * 
     * @warning will update after the next window tick
     * 
     * @param modal say if the window is a modal window or not
     */
    inline void setModal(bool modal) noexcept {m_api->setModal(modal);}

    /**
     * @brief Set the always on top state of the window
     * 
     * @warning will update after the next window tick
     * 
     * @param alwaysOnTop say if the window is always on top or not
     */
    inline void setAlwaysOnTop(bool alwaysOnTop) noexcept {m_api->setAlwaysOnTop(alwaysOnTop);}

    /**
     * @brief Set the Keyboard grabbed state of the window
     * 
     * @warning will update after the next window tick
     * 
     * @param keyboardGrabbed say if the window has the keyboard input grabbed or not
     */
    inline void setKeyboardGrabbed(bool keyboardGrabbed) noexcept {m_api->setKeyboardGrabbed(keyboardGrabbed);}

    /**
     * @brief Set the Focusable state of the window
     * 
     * @warning will update after the next window tick
     * 
     * @param notFocusable say if the window can not be focused or if it can
     */
    inline void setNotFocusable(bool notFocusable) noexcept {m_api->setNotFocusable(notFocusable);}

    /**
     * @brief Set the Relative Mouse mode for this window
     * 
     * @param relativeMode true : the mouse is captured (relative mode enabled) | false : the mouse is not captured (relative mode disabled)
     */
    inline void setRelativeMouseMode(bool relativeMode) noexcept {m_api->setRelativeMouseMode(relativeMode);}

    /**
     * @brief Get the current state of the window settings
     * 
     * @return const WindowSettings& the current window setting state
     */
    inline const WindowSettings& getSettings() const noexcept {return m_api->getSettings();}

    //define SDL / backend stuff
    #ifdef SDL_h_

    /**
     * @brief get the SDL window
     * 
     * @return SDL_Window* a pointer to the SDL window
     */
    inline SDL_Window* getSDLWindow() const noexcept {return (SDL_Window*)m_window;}

    /**
     * @brief get the API window implementation
     * 
     * @return void* a simple pointer to the API implementation
     */
    inline void* getAPI() const noexcept {return m_api;}

    #endif

protected:

    /**
     * @brief store the name of the window
     */
    String m_name = "INVALID";
    /**
     * @brief store the size of the window in pixels
     */
    uivec2 m_size = uivec2(0,0);
    /**
     * @brief store the UUID of the window
     */
    uint32_t m_windowID = UINT32_MAX;
    /**
     * @brief store the actual window
     */
    void* m_window = NULL;
    /**
     * @brief store if closing was requested
     */
    bool m_requestedClosing = false;
    /**
     * @brief store an api-dependent window
     */
    GLGE::Graphic::Backend::API::Window* m_api = nullptr;
    /**
     * @brief store the clear color of the window
     */
    vec4 m_clearColor = vec4(0.5,0.5,0.5,1);
    /**
     * @brief store the value to clear the depth buffer with (NAN disables the clearing)
     */
    float m_depthClear = 1.f;

    /**
     * @brief handle an event for a window
     * 
     * @param event the event to handle
     */
    void handleEvent(const Event* event);

    /**
     * @brief a static function to call for all events
     * 
     * @param event a pointer to the event that occurred
     * @param window a pointer to the window the event is for
     */
    inline static void staticEventHandler(const Event* event, void* window) noexcept
    {((Window*)window)->handleEvent(event);}

};

#endif

#endif