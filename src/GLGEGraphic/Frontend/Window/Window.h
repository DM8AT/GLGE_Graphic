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
#include "../../GLGE_BG/CBinding/String.h"
//add all math types
#include "../../GLGE_Math/GLGEMath.h"

//for C++ add a class
#if __cplusplus

/**
 * @brief store what a simple window is
 */
class Window {
public:

    /**
     * @brief Construct a new Window
     * 
     * @param name the name of the window
     * @param size the size of the window in pixels
     */
    Window(const String& name, const uivec2 size) noexcept;

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
    uint64_t m_windowID = UINT64_MAX;

};

#endif

#endif