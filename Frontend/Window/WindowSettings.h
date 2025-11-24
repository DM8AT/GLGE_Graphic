/**
 * @file WindowSettings.h
 * @author DM8AT
 * @brief define how window settings may look
 * @version 0.1
 * @date 2025-10-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_GRAPHIC_FRONTEND_WINDOW_SETTINGS_
#define _GLGE_GRAPHIC_FRONTEND_WINDOW_SETTINGS_

//for C add booleans
#include <stdbool.h>

#if __cplusplus
extern "C" {
#endif

/**
 * @brief define the window settings
 */
typedef struct s_WindowSettings {
    //say if the window is in fullscreen mode
    bool fullscreen;
    //say if the window is hidden
    bool hidden;
    //say if the window is borderless
    bool borderless;
    //say if the window can be resized by the user
    bool resizable;
    //say if the window is minimized
    bool minimized;
    //say if the window is maximized
    bool maximized;
    //say if the mouse can not go outside the window borders
    bool mouse_grabbed;
    //say if the keyboard input is focused to this window
    bool input_focus;
    //say if the mouse input is focused to this window
    bool mouse_focus;
    //say if the window is modal (must be the child of a window)
    bool modal;
    //say if the window has a high pixel density (high dopts per inch)
    bool high_DPI;
    //say if the window has the mouse captured
    bool mouse_capture;
    //say if the window is always on top
    bool always_on_top;
    //say if the window is a utility window
    bool utility;
    //say if the window is a tooltip window (must be the child of a window)
    bool tooltip;
    //say if the window is a popup menu (must be the child of a window)
    bool popup_menu;
    //say if the keyboard input is grabbed
    bool keyboard_grabbed;
    //say if the window has an transparent framebuffer
    bool transparent;
    //say if the window can not be focused
    bool can_not_focus;
    //say if the window has the mouse in relative mode
    bool relative;
} WindowSettings;

/**
 * @brief define some default settings for the window
 */
const WindowSettings GLGE_WINDOW_SETTINGS_DEFAULT = {
    .fullscreen = false,
    .hidden = false,
    .borderless = false,
    .resizable = true,
    .minimized = false,
    .maximized = false,
    .mouse_grabbed = false,
    .input_focus = true,
    .mouse_focus = true,
    .modal = false,
    .high_DPI = false,
    .mouse_capture = false,
    .always_on_top = false,
    .utility = false,
    .tooltip = false,
    .popup_menu = false,
    .keyboard_grabbed = false,
    .transparent = false,
    .can_not_focus = false,
    .relative = false
};

#if __cplusplus
}
#endif

#endif