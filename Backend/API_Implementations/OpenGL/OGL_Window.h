/**
 * @file OGL_Window.h
 * @author DM8AT
 * @brief define what a window for the OpenGL API looks like
 * @version 0.1
 * @date 2025-10-17
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_GRAPHIC_BACKEND_API_IMPL_OPEN_GL_WINDOW_
#define _GLGE_GRAPHIC_BACKEND_API_IMPL_OPEN_GL_WINDOW_

//add the API window
#include "../API_Window.h"
//add command buffers (needed to add commands)
#include "OGL_CommandBuffer.h"
//add math types
#include "../../../../GLGE_Math/GLGEMath.h"

//only available for C++
#if __cplusplus

//add atomic stuff for thread safe access
#include <atomic>

//use the namespace GLGE::Graphic::Backend::OGL
namespace GLGE::Graphic::Backend::OGL
{

class Window final : public GLGE::Graphic::Backend::API::Window
{
public:

    /**
     * @brief Construct a new OpenGL window
     * 
     * @param window a pointer to the actual window the OpenGL window will belong to
     * @param settings the current settings of the window
     */
    Window(::Window* window, const WindowSettings& settings);

    /**
     * @brief Destroy the OpenGL window
     */
    virtual ~Window() {}

    /**
     * @brief a hook for a function to call when the 
     * 
     */
    virtual void onResize() noexcept override;

    /**
     * @brief make this the current window
     */
    virtual void makeCurrent() noexcept override;

    /**
     * @brief prepare the window for the begining of a frame
     */
    virtual void beginFrame() noexcept override;

    /**
     * @brief clear the window with a clear color
     * 
     * @param clearColor the color to clear with
     */
    void clearWindow(const vec4& clearColor) noexcept;

    /**
     * @brief end the frame for the window
     */
    virtual void endFrame() noexcept override;

    /**
     * @brief update the window at the beginning of the graphics tick
     */
    void update() noexcept;

protected:

    /**
     * @brief store if the window was resized
     */
    std::atomic_bool m_didResize = true;
};

}

#endif

#endif