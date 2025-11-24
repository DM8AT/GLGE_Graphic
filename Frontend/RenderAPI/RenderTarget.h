/**
 * @file RenderTarget.h
 * @author DM8AT
 * @brief define a structure that combines pointers to all elements that can be rendered to
 * @version 0.1
 * @date 2025-11-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//header guard
#ifndef _GLGE_GRAPHIC_FRONTEND_RENDER_API_RENDER_TARGET_
#define _GLGE_GRAPHIC_FRONTEND_RENDER_API_RENDER_TARGET_

//define a type that stores what the render target is
typedef enum e_RenderTargetType {
    //the render target is a window
    GLGE_WINDOW = 0,
    //the render target is a framebuffer
    GLGE_FRAMEBUFFER
} RenderTargetType;

//store a surface that can be rendered to
typedef struct s_RenderTarget {
    //store a pointer to whatever the target is
    void* target;
    //store what the target is
    RenderTargetType type;

    //for C++ add constructors
    #if __cplusplus

    /**
     * @brief Construct a new Render Target
     * 
     * @param _target a pointer to the target
     * @param _type the type of target
     */
    s_RenderTarget(void* _target, RenderTargetType _type)
     : target(_target), type(_type)
    {}

    #endif

} RenderTarget;

#endif