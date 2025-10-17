/**
 * @file OGL_Instance.cpp
 * @author DM8AT
 * @brief implement the OpenGL instance
 * @version 0.1
 * @date 2025-10-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//include the OpenGL instance
#include "OGL_Instance.h"

//use the OpenGL implementation namespace
using namespace GLGE::Graphic::Backend::OGL;

//first, include SDL3 to get access to SDL
#include "SDL3/SDL.h"
//include the frontend windows
#include "../../../Frontend/Window/Window.h"
//add glad
#include "glad/glad.h"

Instance::Instance(Window* window)
 : GLGE::Graphic::Backend::API::Instance()
{
    //create the gl context
    m_glContext = SDL_GL_CreateContext(window->getSDLWindow());
    GLGE_ASSERT((std::string("Failed to create OpenGL context, info: ") + SDL_GetError() + " |"), !m_glContext);

    //initialize GLEW
    GLGE_ASSERT("Failed to initialize GLEW", gladLoadGL() == 0);
}

Instance::~Instance()
{
    
}

uint32_t Instance::getWidnowFlags() noexcept
{
    //return the OpenGL flag
    return SDL_WINDOW_OPENGL;
}