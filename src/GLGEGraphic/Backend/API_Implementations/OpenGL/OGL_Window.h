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

//only available for C++
#if __cplusplus

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
     */
    Window(::Window* window);

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
     * 
     * @param cmdBuff the command buffer to store the command to
     */
    virtual void beginFrame(GLGE::Graphic::Backend::API::CommandBuffer* cmdBuff) noexcept override;

    /**
     * @brief end the frame for the window
     * 
     * @param cmdBuff the command buffer to store the command to
     */
    virtual void endFrame(GLGE::Graphic::Backend::API::CommandBuffer* cmdBuff) noexcept override;

    /**
     * @brief update the window at the beginning of the graphics tick
     */
    void update() noexcept;

};

}

#endif

#endif