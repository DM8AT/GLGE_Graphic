/**
 * @file OGL_Commands.cpp
 * @author DM8AT
 * @brief implement the execute functions for all commands
 * @version 0.1
 * @date 2025-10-19
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//add the commands
#include "OGL_Commands.h"

//add OpenGL
#include "glad/glad.h"

//add SDL3 for window stuff
#include "SDL3/SDL.h"
//include the actual window
#include "../../../Frontend/Window/Window.h"
//also OpenGL windows
#include "OGL_Window.h"

void GLGE::Graphic::Backend::OGL::Command_Clear::execute() noexcept
{
    //just run the clear command
    glClearNamedFramebufferfv(fbuff, buffType, buffId, &r);
}

void GLGE::Graphic::Backend::OGL::Command_Swap::execute() noexcept
{
    //prepare the window swapping
    ((GLGE::Graphic::Backend::OGL::Window*)window->getAPI())->makeCurrent();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //swap the SDL window
    SDL_GL_SwapWindow(window->getSDLWindow());
}

void GLGE::Graphic::Backend::OGL::Command_MakeCurrent::execute() noexcept
{
    //make the window the current window using the built-in function
    ((GLGE::Graphic::Backend::OGL::Window*)window->getAPI())->makeCurrent();
}