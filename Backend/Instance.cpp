/**
 * @file Instance.cpp
 * @author DM8AT
 * @brief implement the functionality for graphic instances
 * @version 0.1
 * @date 2025-10-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//include graphic instances
#include "Instance.h"
//SDL3 is required
#include "SDL3/SDL.h"
#include "SDL3/SDL_mouse.h"
//include the debugging from the background library
#include "../../GLGE_BG/Debugging/Logging/__BG_SimpleDebug.h"

//include the windows for the window event types
#include "../Frontend/Window/Window.h"

//include all backends
#include "API_Implementations/AllImplementations.h"

//locally use the backend namespace
using namespace GLGE::Graphic::Backend;

//utility functions

/**
 * @brief map an SDL key code to an GLGE key code
 * 
 * @param key the SDL key that was triggered
 * @return Key the GLGE key that maps to that key
 */
static Key __mapSDLKeyToGLGEKey(const SDL_Keycode& key) noexcept {
    //just use a switch - case for mapping
    switch (key)
    {
    
    // --- LETTER KEYS ---

    case SDLK_A: return GLGE_KEY_a;
    case SDLK_B: return GLGE_KEY_b;
    case SDLK_C: return GLGE_KEY_c;
    case SDLK_D: return GLGE_KEY_d;
    case SDLK_E: return GLGE_KEY_e;
    case SDLK_F: return GLGE_KEY_f;
    case SDLK_G: return GLGE_KEY_g;
    case SDLK_H: return GLGE_KEY_h;
    case SDLK_I: return GLGE_KEY_i;
    case SDLK_J: return GLGE_KEY_j;
    case SDLK_K: return GLGE_KEY_k;
    case SDLK_L: return GLGE_KEY_l;
    case SDLK_M: return GLGE_KEY_m;
    case SDLK_N: return GLGE_KEY_n;
    case SDLK_O: return GLGE_KEY_o;
    case SDLK_P: return GLGE_KEY_p;
    case SDLK_Q: return GLGE_KEY_q;
    case SDLK_R: return GLGE_KEY_r;
    case SDLK_S: return GLGE_KEY_s;
    case SDLK_T: return GLGE_KEY_t;
    case SDLK_U: return GLGE_KEY_u;
    case SDLK_V: return GLGE_KEY_v;
    case SDLK_W: return GLGE_KEY_w;
    case SDLK_X: return GLGE_KEY_x;
    case SDLK_Y: return GLGE_KEY_y;
    case SDLK_Z: return GLGE_KEY_z;

    // --- SPECIAL KEYS ---

    case SDLK_LSHIFT: return GLGE_KEY_LEFT_SHIFT;
    case SDLK_RSHIFT: return GLGE_KEY_RIGHT_SHIFT;
    case SDLK_LCTRL: return GLGE_KEY_LEFT_CTRL;
    case SDLK_RCTRL: return GLGE_KEY_RIGHT_CTRL;
    case SDLK_LALT: return GLGE_KEY_LEFT_ALT;
    case SDLK_RALT: return GLGE_KEY_RIGHT_ALT;
    case SDLK_LGUI: return GLGE_KEY_LEFT_GUI;
    case SDLK_RGUI: return GLGE_KEY_RIGHT_GUI;
    case SDLK_SPACE: return GLGE_KEY_SPACE;
    case SDLK_CAPSLOCK: return GLGE_KEY_CAPS_LOCK;
    case SDLK_TAB: return GLGE_KEY_TAB;
    case SDLK_BACKSPACE: return GLGE_KEY_BACKSPACE;
    case SDLK_RETURN: return GLGE_KEY_ENTER;
    case SDLK_MODE: return GLGE_KEY_MODE;
    case SDLK_SLEEP: return GLGE_KEY_SLEEP;
    case SDLK_WAKE: return GLGE_KEY_WAKE;
    case SDLK_ESCAPE: return GLGE_KEY_ESCAPE;

    // --- CHANNEL / SOFT KEYS ---
    case SDLK_CHANNEL_INCREMENT: return GLGE_KEY_CHANNEL_INCREMENT;
    case SDLK_CHANNEL_DECREMENT: return GLGE_KEY_CHANNEL_DECREMENT;
    case SDLK_SOFTLEFT: return GLGE_KEY_SOFT_LEFT;
    case SDLK_SOFTRIGHT: return GLGE_KEY_SOFT_RIGHT;

    case SDLK_CALL: return GLGE_KEY_CALL;
    case SDLK_ENDCALL: return GLGE_KEY_END_CALL;

    // --- MEDIA KEYS ---
    case SDLK_MEDIA_PLAY: return GLGE_KEY_MEDIA_PLAY;
    case SDLK_MEDIA_PAUSE: return GLGE_KEY_MEDIA_PAUSE;
    case SDLK_MEDIA_RECORD: return GLGE_KEY_MEDIA_RECORD;
    case SDLK_MEDIA_FAST_FORWARD: return GLGE_KEY_MEDIA_FAST_FORWARD;
    case SDLK_MEDIA_REWIND: return GLGE_KEY_MEDIA_REWIND;
    case SDLK_MEDIA_NEXT_TRACK: return GLGE_KEY_MEDIA_NEXT_TRACK;
    case SDLK_MEDIA_PREVIOUS_TRACK: return GLGE_KEY_MEDIA_PREVIOUS_TRACK;
    case SDLK_MEDIA_STOP: return GLGE_KEY_MEDIA_STOP;
    case SDLK_MEDIA_EJECT: return GLGE_KEY_MEDIA_EJECT;
    case SDLK_MEDIA_PLAY_PAUSE: return GLGE_KEY_MEDIA_PLAY_PAUSE;
    case SDLK_MEDIA_SELECT: return GLGE_KEY_MEDIA_SELECT;

    // --- APPLICATION CONTROLS ---
    case SDLK_AC_NEW: return GLGE_KEY_APP_CTRL_NEW;
    case SDLK_AC_OPEN: return GLGE_KEY_APP_CTRL_OPEN;
    case SDLK_AC_CLOSE: return GLGE_KEY_APP_CTRL_CLOSE;
    case SDLK_AC_EXIT: return GLGE_KEY_APP_CTRL_EXIT;
    case SDLK_AC_SAVE: return GLGE_KEY_APP_CTRL_SAVE;
    case SDLK_AC_PRINT: return GLGE_KEY_APP_CTRL_PRINT;
    case SDLK_AC_PROPERTIES: return GLGE_KEY_APP_CTRL_PROPERTIES;
    case SDLK_AC_SEARCH: return GLGE_KEY_APP_CTRL_SEARCH;
    case SDLK_AC_HOME: return GLGE_KEY_APP_CTRL_HOME;
    case SDLK_AC_BACK: return GLGE_KEY_APP_CTRL_BACK;
    case SDLK_AC_FORWARD: return GLGE_KEY_APP_CTRL_FORWARD;
    case SDLK_AC_STOP: return GLGE_KEY_APP_CTRL_STOP;
    case SDLK_AC_REFRESH: return GLGE_KEY_APP_CTRL_REFRESH;
    case SDLK_AC_BOOKMARKS: return GLGE_KEY_APP_CTRL_BOOKMARKS;

    // --- SYMBOLS ---
    case SDLK_EXCLAIM: return GLGE_KEY_EXCLAIM;
    case SDLK_HASH: return GLGE_KEY_HASH;
    case SDLK_DOLLAR: return GLGE_KEY_DOLLAR;
    case SDLK_PERCENT: return GLGE_KEY_PERCENT;
    case SDLK_AMPERSAND: return GLGE_KEY_AMPERSAND;
    case SDLK_APOSTROPHE: return GLGE_KEY_APOSTROPHE;
    case SDLK_LEFTPAREN: return GLGE_KEY_LEFT_PRENTICES;
    case SDLK_RIGHTPAREN: return GLGE_KEY_RIGHT_PRENTICES;
    case SDLK_ASTERISK: return GLGE_KEY_ASTERISK;
    case SDLK_PLUS: return GLGE_KEY_PLUS;
    case SDLK_COMMA: return GLGE_KEY_COMMA;
    case SDLK_MINUS: return GLGE_KEY_MINUS;
    case SDLK_PERIOD: return GLGE_KEY_PERIOD;
    case SDLK_SLASH: return GLGE_KEY_SLASH;
    case SDLK_COLON: return GLGE_KEY_COLON;
    case SDLK_SEMICOLON: return GLGE_KEY_SEMICOLON;
    case SDLK_LESS: return GLGE_KEY_LESS;
    case SDLK_EQUALS: return GLGE_KEY_EQUALS;
    case SDLK_GREATER: return GLGE_KEY_GREATER;
    case SDLK_QUESTION: return GLGE_KEY_QUESTION;
    case SDLK_AT: return GLGE_KEY_AT;
    case SDLK_LEFTBRACKET: return GLGE_KEY_LEFT_BRACKET;
    case SDLK_BACKSLASH: return GLGE_KEY_BACKSLASH;
    case SDLK_RIGHTBRACKET: return GLGE_KEY_RIGHT_BRACKET;
    case SDLK_CARET: return GLGE_KEY_CARET;
    case SDLK_UNDERSCORE: return GLGE_KEY_UNDERSCORE;
    case SDLK_GRAVE: return GLGE_KEY_GRAVE;
    case SDLK_LEFTBRACE: return GLGE_KEY_LEFT_BRACE;
    case SDLK_PIPE: return GLGE_KEY_PIPE;
    case SDLK_RIGHTBRACE: return GLGE_KEY_RIGHT_BRACE;
    case SDLK_TILDE: return GLGE_KEY_TILDE;
    case SDLK_PLUSMINUS: return GLGE_KEY_PLUS_MINUS;

    // --- FUNCTION KEYS ---
    case SDLK_PRINTSCREEN: return GLGE_KEY_PRINT_SCREEN;
    case SDLK_SCROLLLOCK: return GLGE_KEY_SCROLL_LOCK;
    case SDLK_PAUSE: return GLGE_KEY_PAUSE;

    case SDLK_INSERT: return GLGE_KEY_INSERT;
    case SDLK_HOME: return GLGE_KEY_HOME;
    case SDLK_PAGEUP: return GLGE_KEY_PAGE_UP;
    case SDLK_PAGEDOWN: return GLGE_KEY_PAGE_DOWN;
    case SDLK_END: return GLGE_KEY_END;
    case SDLK_RIGHT: return GLGE_KEY_RIGHT;
    case SDLK_LEFT: return GLGE_KEY_LEFT;
    case SDLK_DOWN: return GLGE_KEY_DOWN;
    case SDLK_UP: return GLGE_KEY_UP;

    case SDLK_NUMLOCKCLEAR: return GLGE_KEY_NUM_LOCK_CLEAR;
    case SDLK_APPLICATION: return GLGE_KEY_APPLICATION;
    case SDLK_POWER: return GLGE_KEY_POWER;
    case SDLK_EXECUTE: return GLGE_KEY_EXECUTE;
    case SDLK_HELP: return GLGE_KEY_HELP;
    case SDLK_MENU: return GLGE_KEY_MENU;
    case SDLK_SELECT: return GLGE_KEY_SELECT;
    case SDLK_STOP: return GLGE_KEY_STOP;
    case SDLK_AGAIN: return GLGE_KEY_AGAIN;
    case SDLK_UNDO: return GLGE_KEY_UNDO;
    case SDLK_CUT: return GLGE_KEY_CUT;
    case SDLK_COPY: return GLGE_KEY_COPY;
    case SDLK_PASTE: return GLGE_KEY_PASTE;
    case SDLK_FIND: return GLGE_KEY_FIND;
    case SDLK_MUTE: return GLGE_KEY_MUTE;
    case SDLK_VOLUMEUP: return GLGE_KEY_VOLUME_UP;
    case SDLK_VOLUMEDOWN: return GLGE_KEY_VOLUME_DOWN;
    case SDLK_ALTERASE: return GLGE_KEY_ALT_ERASE;
    case SDLK_SYSREQ: return GLGE_KEY_SYS_REQ;
    case SDLK_CLEAR: return GLGE_KEY_CLEAR;

    case SDLK_PRIOR: return GLGE_KEY_PRIOR;
    case SDLK_RETURN2: return GLGE_KEY_RETURN;
    case SDLK_SEPARATOR: return GLGE_KEY_SEPARATOR;
    case SDLK_OUT: return GLGE_KEY_OUT;
    case SDLK_OPER: return GLGE_KEY_OPER;
    case SDLK_CLEARAGAIN: return GLGE_KEY_CLEAR_AGAIN;
    case SDLK_CRSEL: return GLGE_KEY_CRSEL;
    case SDLK_EXSEL: return GLGE_KEY_EXSEL;

    case SDLK_THOUSANDSSEPARATOR: return GLGE_KEY_THOUSANDS_SEPARATOR;
    case SDLK_DECIMALSEPARATOR: return GLGE_KEY_DECIMAL_SEPARATOR;
    case SDLK_CURRENCYUNIT: return GLGE_KEY_CURRENCY_UNIT;
    case SDLK_CURRENCYSUBUNIT: return GLGE_KEY_CURRENCY_SUBUNIT;

    // --- NUMPAD ---
    case SDLK_KP_DIVIDE: return GLGE_KEY_NUMPAD_DIVIDE;
    case SDLK_KP_MULTIPLY: return GLGE_KEY_NUMPAD_MULTIPLY;
    case SDLK_KP_MINUS: return GLGE_KEY_NUMPAD_MINUS;
    case SDLK_KP_PLUS: return GLGE_KEY_NUMPAD_PLUS;
    case SDLK_KP_ENTER: return GLGE_KEY_NUMPAD_ENTER;
    case SDLK_KP_0: return GLGE_KEY_NUMPAD_0;
    case SDLK_KP_1: return GLGE_KEY_NUMPAD_1;
    case SDLK_KP_2: return GLGE_KEY_NUMPAD_2;
    case SDLK_KP_3: return GLGE_KEY_NUMPAD_3;
    case SDLK_KP_4: return GLGE_KEY_NUMPAD_4;
    case SDLK_KP_5: return GLGE_KEY_NUMPAD_5;
    case SDLK_KP_6: return GLGE_KEY_NUMPAD_6;
    case SDLK_KP_7: return GLGE_KEY_NUMPAD_7;
    case SDLK_KP_8: return GLGE_KEY_NUMPAD_8;
    case SDLK_KP_9: return GLGE_KEY_NUMPAD_9;
    case SDLK_KP_PERIOD: return GLGE_KEY_NUMPAD_PERIOD;
    case SDLK_KP_EQUALS: return GLGE_KEY_NUMPAD_EQUALS;
    case SDLK_KP_COMMA: return GLGE_KEY_NUMPAD_COMMA;
    case SDLK_KP_EQUALSAS400: return GLGE_KEY_NUMPAD_EQUALS_AS_400;

    case SDLK_KP_00: return GLGE_KEY_NUMPAD_00;
    case SDLK_KP_000: return GLGE_KEY_NUMPAD_000;
    case SDLK_KP_LEFTPAREN: return GLGE_KEY_NUMPAD_LEFT_PRENTICES;
    case SDLK_KP_RIGHTPAREN: return GLGE_KEY_NUMPAD_RIGHT_PRENTICES;
    case SDLK_KP_LEFTBRACE: return GLGE_KEY_NUMPAD_LEFT_BRACE;
    case SDLK_KP_RIGHTBRACE: return GLGE_KEY_NUMPAD_RIGHT_BRACE;
    case SDLK_KP_TAB: return GLGE_KEY_NUMPAD_TAB;
    case SDLK_KP_BACKSPACE: return GLGE_KEY_NUMPAD_BACKSPACE;

    case SDLK_KP_A: return GLGE_KEY_NUMPAD_A;
    case SDLK_KP_B: return GLGE_KEY_NUMPAD_B;
    case SDLK_KP_C: return GLGE_KEY_NUMPAD_C;
    case SDLK_KP_D: return GLGE_KEY_NUMPAD_D;
    case SDLK_KP_E: return GLGE_KEY_NUMPAD_E;
    case SDLK_KP_F: return GLGE_KEY_NUMPAD_F;

    case SDLK_KP_XOR: return GLGE_KEY_NUMPAD_XOR;
    case SDLK_KP_POWER: return GLGE_KEY_NUMPAD_POWER;
    case SDLK_KP_PERCENT: return GLGE_KEY_NUMPAD_PERCENT;
    case SDLK_KP_LESS: return GLGE_KEY_NUMPAD_LESS;
    case SDLK_KP_GREATER: return GLGE_KEY_NUMPAD_GREATER;
    case SDLK_KP_AMPERSAND: return GLGE_KEY_NUMPAD_AMPERSAND;
    case SDLK_KP_DBLAMPERSAND: return GLGE_KEY_NUMPAD_DBL_AMPERSAND;
    case SDLK_KP_VERTICALBAR: return GLGE_KEY_NUMPAD_VERTICAL_BAR;
    case SDLK_KP_DBLVERTICALBAR: return GLGE_KEY_NUMPAD_DBL_VERTICAL_BAR;

    case SDLK_KP_COLON: return GLGE_KEY_NUMPAD_COLON;
    case SDLK_KP_HASH: return GLGE_KEY_NUMPAD_HASH;
    case SDLK_KP_SPACE: return GLGE_KEY_NUMPAD_SPACE;
    case SDLK_KP_AT: return GLGE_KEY_NUMPAD_AT;
    case SDLK_KP_EXCLAM: return GLGE_KEY_NUMPAD_EXCLAM;
    case SDLK_KP_MEMSTORE: return GLGE_KEY_NUMPAD_MEM_STORE;
    case SDLK_KP_MEMRECALL: return GLGE_KEY_NUMPAD_MEM_RECALL;
    case SDLK_KP_MEMCLEAR: return GLGE_KEY_NUMPAD_MEM_CLEAR;
    case SDLK_KP_MEMADD: return GLGE_KEY_NUMPAD_MEM_ADD;
    case SDLK_KP_MEMSUBTRACT: return GLGE_KEY_NUMPAD_MEM_SUBTRACT;
    case SDLK_KP_MEMMULTIPLY: return GLGE_KEY_NUMPAD_MEM_MULTIPLY;
    case SDLK_KP_MEMDIVIDE: return GLGE_KEY_NUMPAD_MEM_DIVIDE;
    case SDLK_KP_PLUSMINUS: return GLGE_KEY_NUMPAD_PLUS_MINUS;
    case SDLK_KP_CLEAR: return GLGE_KEY_NUMPAD_CLEAR;
    case SDLK_KP_CLEARENTRY: return GLGE_KEY_NUMPAD_CLEAR_ENTRY;
    case SDLK_KP_BINARY: return GLGE_KEY_NUMPAD_BINARY;
    case SDLK_KP_OCTAL: return GLGE_KEY_NUMPAD_OCTAL;
    case SDLK_KP_DECIMAL: return GLGE_KEY_NUMPAD_DECIMAL;
    case SDLK_KP_HEXADECIMAL: return GLGE_KEY_NUMPAD_HEXADECIMAL;

    // --- FUNCTION KEYS ---
    case SDLK_F1: return GLGE_KEY_F1;
    case SDLK_F2: return GLGE_KEY_F2;
    case SDLK_F3: return GLGE_KEY_F3;
    case SDLK_F4: return GLGE_KEY_F4;
    case SDLK_F5: return GLGE_KEY_F5;
    case SDLK_F6: return GLGE_KEY_F6;
    case SDLK_F7: return GLGE_KEY_F7;
    case SDLK_F8: return GLGE_KEY_F8;
    case SDLK_F9: return GLGE_KEY_F9;
    case SDLK_F10: return GLGE_KEY_F10;
    case SDLK_F11: return GLGE_KEY_F11;
    case SDLK_F12: return GLGE_KEY_F12;
    case SDLK_F13: return GLGE_KEY_F13;
    case SDLK_F14: return GLGE_KEY_F14;
    case SDLK_F15: return GLGE_KEY_F15;
    case SDLK_F16: return GLGE_KEY_F16;
    case SDLK_F17: return GLGE_KEY_F17;
    case SDLK_F18: return GLGE_KEY_F18;
    case SDLK_F19: return GLGE_KEY_F19;
    case SDLK_F20: return GLGE_KEY_F20;
    case SDLK_F21: return GLGE_KEY_F21;
    case SDLK_F22: return GLGE_KEY_F22;
    case SDLK_F23: return GLGE_KEY_F23;
    case SDLK_F24: return GLGE_KEY_F24;

    // --- Digits ---
    case SDLK_0: return GLGE_KEY_0;
    case SDLK_1: return GLGE_KEY_1;
    case SDLK_2: return GLGE_KEY_2;
    case SDLK_3: return GLGE_KEY_3;
    case SDLK_4: return GLGE_KEY_4;
    case SDLK_5: return GLGE_KEY_5;
    case SDLK_6: return GLGE_KEY_6;
    case SDLK_7: return GLGE_KEY_7;
    case SDLK_8: return GLGE_KEY_8;
    case SDLK_9: return GLGE_KEY_9;

    default:
        //just return garbage
        return GLGE_KEY_ENUM_MAX;
        break;
    }
}

//implement the instance

Instance::Instance()
 : m_windowEventStack(std::vector<LayerBase*>()),
   m_eventStack(std::vector<LayerBase*>{&m_keyLayer, &m_miceLayer})
{
    //set the default hints for GLGE
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "0");
    SDL_SetHint(SDL_HINT_MAC_CTRL_CLICK_EMULATE_RIGHT_CLICK, "1");
    SDL_SetHint(SDL_HINT_WINDOW_ACTIVATE_WHEN_RAISED, "1");
    SDL_SetHint(SDL_HINT_MOUSE_TOUCH_EVENTS, "1");
    SDL_SetHint(SDL_HINT_VIDEO_SYNC_WINDOW_OPERATIONS, "0");
    SDL_SetHint(SDL_HINT_VIDEO_WAYLAND_SCALE_TO_DISPLAY, "1");
    SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "1");
    SDL_SetHint(SDL_HINT_VIDEO_SYNC_WINDOW_OPERATIONS, "0");
    //initialize SDL3 (this thread is the main thread as this is a static type)
    SDL_InitSubSystem(SDL_INIT_VIDEO);

    //initialize the mice layer
    m_miceLayer.init();
}

Instance::~Instance()
{
    //stop SDL
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_Quit();
}

void Instance::update() noexcept
{
    //update the layer stacks
    m_windowEventStack.update();
    m_eventStack.update();

    //poll all SDL events
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        //store if the event should be skipped
        bool skip = false;
        //call all callbacks
        for (const auto& callback : m_SDLEventCallbacks) 
        {if ((*callback.second)(&e)) {skip = true; break;}}
        //if skipping is requested, skip
        if (skip) {continue;}
        
        //check the SDL event
        switch (e.type)
        {
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            //handle the window close request event
            m_windowEventStack.sendEvent(Event(WINDOW_EVENT_TYPE_CLOSE_REQUEST, EventData(&e.window.windowID, sizeof(e.window.windowID))));
            break;
        case SDL_EVENT_WINDOW_MINIMIZED:
            //handle the minimized event
            m_windowEventStack.sendEvent(Event(WINDOW_EVENT_TYPE_MINIMIZED, EventData(&e.window.windowID, sizeof(e.window.windowID))));
            break;
        case SDL_EVENT_WINDOW_RESTORED:
            //handle the window restoring
            m_windowEventStack.sendEvent(Event(WINDOW_EVENT_TYPE_RESTORED, EventData(&e.window.windowID, sizeof(e.window.windowID))));
            break;
        case SDL_EVENT_WINDOW_MAXIMIZED: 
            //handle the window maximizing
            m_windowEventStack.sendEvent(Event(WINDOW_EVENT_TYPE_MAXIMIZED, EventData(&e.window.windowID, sizeof(e.window.windowID))));
            break;
        case SDL_EVENT_WINDOW_RESIZED:
            //handle the window resizing
            m_windowEventStack.sendEvent(Event(WINDOW_EVENT_TYPE_SIZE_CHANGE, EventData(&e.window, sizeof(e.window))));
            break;

        case SDL_EVENT_KEY_DOWN: {
            //store the key
            Key key = __mapSDLKeyToGLGEKey(e.key.key);
            //send the key down event
            m_eventStack.sendEvent(Event(GLGE_KEY_EVENT_KEY_DOWN, EventData(1, (uint8_t*)&key)));
            }
            break;
        case SDL_EVENT_KEY_UP: {
            //store the key
            Key key = __mapSDLKeyToGLGEKey(e.key.key);
            //send the key up event
            m_eventStack.sendEvent(Event(GLGE_KEY_EVENT_KEY_UP, EventData(1, (uint8_t*)&key)));
            }
            break;

        case SDL_EVENT_MOUSE_MOTION: {
            //store the required data
            GLGE_MOUSE_EVENT_DATA data{
                .mouse = e.motion.which,
                .data{.pos = ivec2(
                    e.motion.x, 
                    e.motion.y
                )}
            };
            //send the mouse event
            m_eventStack.sendEvent(Event(GLGE_MOUSE_EVENT_MOUSE_MOVE, EventData{sizeof(data), (byte*)&data}));
            }
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            //store the required data
            GLGE_MOUSE_EVENT_DATA data{
                .mouse = e.button.which,
                .data{.button = e.button.button}
            };
            //send the mouse event
            m_eventStack.sendEvent(Event(GLGE_MOUSE_EVENT_MOUSE_DOWN, EventData(sizeof(data), (byte*)&data)));
            }
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP: {
            //store the required data
            GLGE_MOUSE_EVENT_DATA data{
                .mouse = e.button.which,
                .data{.button = e.button.button}
            };
            //send the mouse event
            m_eventStack.sendEvent(Event(GLGE_MOUSE_EVENT_MOUSE_UP, EventData(sizeof(data), (byte*)&data)));
            }
            break;

        default:
            break;
        }
    }

    //update the specific mice states
    {
        //update the global mouse
        //get data for the global mouse state
        vec2 pos;
        SDL_MouseButtonFlags buttons = SDL_GetGlobalMouseState(&pos.x, &pos.y);
        //store the global mouse state
        m_globalMouse.pressed[0] = (uint64_t)buttons & ~(uint64_t)m_globalMouse.current[0];
        m_globalMouse.released[0] = ~(uint64_t)buttons & (uint64_t)m_globalMouse.current[0];
        m_globalMouse.current[0] = (uint64_t)buttons;
        m_globalMouse.pixelPos = ivec2(pos.x, pos.y);

        //update the relative mouse state
        //get the data for the relative mouse state
        buttons = SDL_GetRelativeMouseState(&pos.x, &pos.y);
        //store the relative mouse state
        m_relativeMouse.pressed[0] = (uint64_t)buttons & ~(uint64_t)m_relativeMouse.current[0];
        m_relativeMouse.released[0] = ~(uint64_t)buttons & (uint64_t)m_relativeMouse.current[0];
        m_relativeMouse.current[0] = (uint64_t)buttons;
        m_relativeMouse.pixelPos = ivec2(pos.x, pos.y);
    }

    //tick the backend instance
    m_inst->tick();
    m_inst->onUpdate();
}

void Instance::registerWindow(Window* window) noexcept
{
    //add the window as an input layer
    if (!m_windowEventStack.addNewLayer(window)) {
        //fatal error
        GLGE_ABORT("[CRITICAL ERROR] Failed to register the window as an event layer to the base instance");
    }

    //check if this was the first window
    if (m_activeWindowCount == 0) {
        //check if a new instance must created (this should be the case)
        GLGE_ASSERT("No window exists, but the graphic API is initialized. This should not be the case. ", 
                    (m_inst != nullptr) && (m_inst != (GLGE::Graphic::Backend::API::Instance*)1));

        //create the new instance
        //first, get rid of the auto api
        resolveAPI();

        //switch over the api and select the correct one
        switch (m_api)
        {
        case GLGE_GRAPHIC_INSTANCE_API_OPEN_GL:
            m_inst = new GLGE::Graphic::Backend::OGL::Instance(window);
            break;
        
        default:
            break;
        }
    }

    //increase the amount of known windows
    ++m_activeWindowCount;
}

void Instance::deregisterWindow(Window* window) noexcept
{
    //get the ID of the window
    uint64_t id = m_windowEventStack.getLayerIndex(window->getType().library, window->getType().name);
    //sanity check of the ID
    if (id == UINT64_MAX) {
        //debug message
        GLGE_DEBUG_MESSAGE("Invalid window ID: Window de-registration skipped");
        return;
    }

    //remove the layer
    m_windowEventStack.removeLayer(id);
    //decrease the amount of known windows
    --m_activeWindowCount;

    //check if this was the last window
    if (m_activeWindowCount == 0) {
        //de-register the graphic API
        if (m_inst) {
            delete m_inst;
            m_inst = nullptr;
        }
    }
}

uint32_t Instance::getWindowFlags() noexcept
{
    //if the instance is NULL, set it to 1 to lock the API
    if (!m_inst) {m_inst = (GLGE::Graphic::Backend::API::Instance*)1;}
    //if the API is auto, resolve the API
    resolveAPI();
    //switch over the API
    switch (m_api)
    {
    case GLGE_GRAPHIC_INSTANCE_API_OPEN_GL:
        //return the OpenGL window flags
        return GLGE::Graphic::Backend::OGL::Instance::getWindowFlags();
        break;
    
    default:
        break;
    }
    //just use no flags
    return 0;
}

void Instance::resolveAPI() noexcept
{
    //if the API is not auto, just stop
    if (m_api != GLGE_GRAPHIC_INSTANCE_API_AUTO) {return;}

    //for now, just use OpenGL
    m_api = GLGE_GRAPHIC_INSTANCE_API_OPEN_GL;
}



//initialize the global graphic instance
Instance GLGE::Graphic::Backend::INSTANCE = Instance();


/**
 * @brief store a direct mapping from API enum value to 
 */
static bool GLGE_SUPPORTED_API_MAP[] = {
    true,
    true,
    false,
    false
};

bool glge_graphic_IsAPISupported(InstanceAPI api) {
    //simply return the value from the support map (for now)
    return GLGE_SUPPORTED_API_MAP[(int)api];
}