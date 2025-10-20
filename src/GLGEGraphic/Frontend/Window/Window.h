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
     */
    Window(const String& name, const uivec2 size) noexcept;

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