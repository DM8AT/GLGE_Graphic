/**
 * @file GLGE_ImGuiImpl.h
 * @author DM8AT
 * @brief define an API for an extension that adds ImGui to GLGE
 * @version 0.1
 * @date 2025-11-21
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//header guard
#ifndef _GLGE_IMGUI_IMP_
#define _GLGE_IMGUI_IMP_

//GLGE is required for interfacing
#include "GLGEGraphic/GLGEGraphic.h"

/**
 * @brief initialize the ImGui implementation with a window
 * 
 * @param win a constant reference to the window to initialize from
 */
void glge_ImGui_init(const Window& win);

/**
 * @brief shutdown the ImGui implementation
 */
void glge_ImGui_Shutdown();

/**
 * @brief a function to insert to a render pipeline as a function to start a frame
 */
void glge_ImGui_FrameStart(void*);

/**
 * @brief a function to insert to a render pipeline as a function to end a frame
 */
void glge_ImGui_FrameEnd(void*);

#endif