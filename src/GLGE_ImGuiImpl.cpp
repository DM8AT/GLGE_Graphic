/**
 * @file GLGE_ImGuiImpl.cpp
 * @author DM8AT
 * @brief implement the ImGui binding
 * @version 0.1
 * @date 2025-11-21
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_opengl3.h"
//including SDL3 allows for backend access
#include <SDL3/SDL.h>
#include "GLGEGraphic/external/glad/include/glad/glad.h"

//add the imgui API
#include "GLGE_ImGuiImpl.h"

//store the ImGui io data
static ImGuiIO* io = nullptr;

//a function to call for SDL events
static bool __glge_ImGui_HandleSDLEvent(void* event) noexcept {
    //parse the event
    ImGui_ImplSDL3_ProcessEvent((SDL_Event*)event);
    //check if the element is a mouse event or a keyboard event or nothing of them
    switch (((SDL_Event*)event)->type)
    {
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
        //stop the event if the keyboard is captured
        return io->WantCaptureKeyboard;

    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
    case SDL_EVENT_MOUSE_MOTION:
    case SDL_EVENT_MOUSE_WHEEL:
        //stop the event if the mouse is captured
        return io->WantCaptureMouse;
    
    default:
        //do not stop the even
        return false;
        break;
    }
}

void glge_ImGui_init(const Window& win) {
    //create the context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    //get the input / output system
    io = &ImGui::GetIO();

    //init the backends
    ImGui_ImplSDL3_InitForOpenGL(win.getSDLWindow(), glge_Graphic_GetOpenGLContext());
    ImGui_ImplOpenGL3_Init("#version 330 core");

    //add the event callback
    glge_Graphic_AddSDLEventCallback("GLGE_IMGUI_CALLBACK", __glge_ImGui_HandleSDLEvent);
}

void glge_ImGui_Shutdown() {
    //remove the event callback
    glge_Graphic_RemoveSDLEventCallback("GLGE_IMGUI_CALLBACK");

    //shutdown the backends
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    //delete the context
    ImGui::DestroyContext();
}

void glge_ImGui_FrameStart(void*) {
    //start a new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}

void glge_ImGui_FrameEnd(void*) {
    //render the frame
    ImGui::Render();
    //do the OpenGL rendering
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}