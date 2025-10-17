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

protected:

    /**
     * @brief store a pointer to the frontend window
     */
    ::Window* m_window = nullptr;

};

};

#endif

#endif