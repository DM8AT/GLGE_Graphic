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

//for C++ add a class
#if __cplusplus

//include string views for utilities
#include <string_view>
//store the library for the window
constexpr std::string_view WINDOW_LIBRARY_NAME = "GLGE_GRAPHIC";
//store the identifier for the window
constexpr std::string_view WINDOW_LAYER_NAME = "WINDOW_LAYER";

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
     * @brief get if closing was requested
     * 
     * @return true : closing was requested
     * @return false : closing is not requested
     */
    inline bool isClosingRequested() const noexcept {return m_requestedClosing;}

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