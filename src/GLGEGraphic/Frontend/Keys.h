/**
 * @file Keys.h
 * @author DM8AT
 * @brief define an API to quarry keys
 * @version 0.1
 * @date 2025-11-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//header guard
#ifndef _GLGE_GRAPHIC_FRONTEND_KEYS_
#define _GLGE_GRAPHIC_FRONTEND_KEYS_

//everything is available in C
#if __cplusplus
extern "C" {
#endif

//add types
#include "../../../GLGE_Core/Types.h"

/**
 * @brief 
 */
typedef enum e_Key {
    //lower case letters

    GLGE_KEY_a = 0,
    GLGE_KEY_b,
    GLGE_KEY_c,
    GLGE_KEY_d,
    GLGE_KEY_e,
    GLGE_KEY_f,
    GLGE_KEY_g,
    GLGE_KEY_h,
    GLGE_KEY_i,
    GLGE_KEY_j,
    GLGE_KEY_k,
    GLGE_KEY_l,
    GLGE_KEY_m,
    GLGE_KEY_n,
    GLGE_KEY_o,
    GLGE_KEY_p,
    GLGE_KEY_q,
    GLGE_KEY_r,
    GLGE_KEY_s,
    GLGE_KEY_t,
    GLGE_KEY_u,
    GLGE_KEY_v,
    GLGE_KEY_w,
    GLGE_KEY_x,
    GLGE_KEY_y,
    GLGE_KEY_z,

    //command keys

    GLGE_KEY_LEFT_SHIFT,
    GLGE_KEY_RIGHT_SHIFT,
    GLGE_KEY_LEFT_CTRL,
    GLGE_KEY_RIGHT_CTRL,
    GLGE_KEY_LEFT_ALT,
    GLGE_KEY_RIGHT_ALT,
    GLGE_KEY_LEFT_GUI,
    GLGE_KEY_RIGHT_GUI,
    GLGE_KEY_SPACE,
    GLGE_KEY_CAPS_LOCK,
    GLGE_KEY_TAB,
    GLGE_KEY_BACKSPACE,
    GLGE_KEY_ENTER,
    GLGE_KEY_MODE,
    GLGE_KEY_SLEEP,
    GLGE_KEY_WAKE,
    GLGE_KEY_CHANNEL_INCREMENT,
    GLGE_KEY_CHANNEL_DECREMENT,
    GLGE_KEY_SOFT_LEFT,
    GLGE_KEY_SOFT_RIGHT,
    GLGE_KEY_CALL,
    GLGE_KEY_END_CALL,

    //media

    GLGE_KEY_MEDIA_PLAY,
    GLGE_KEY_MEDIA_PAUSE,
    GLGE_KEY_MEDIA_RECORD,
    GLGE_KEY_MEDIA_FAST_FORWARD,
    GLGE_KEY_MEDIA_REWIND,
    GLGE_KEY_MEDIA_NEXT_TRACK,
    GLGE_KEY_MEDIA_PREVIOUS_TRACK,
    GLGE_KEY_MEDIA_STOP,
    GLGE_KEY_MEDIA_EJECT,
    GLGE_KEY_MEDIA_PLAY_PAUSE,
    GLGE_KEY_MEDIA_SELECT,

    //app control

    GLGE_KEY_APP_CTRL_NEW,
    GLGE_KEY_APP_CTRL_OPEN,
    GLGE_KEY_APP_CTRL_CLOSE,
    GLGE_KEY_APP_CTRL_EXIT,
    GLGE_KEY_APP_CTRL_SAVE,
    GLGE_KEY_APP_CTRL_PRINT,
    GLGE_KEY_APP_CTRL_PROPERTIES,
    GLGE_KEY_APP_CTRL_SEARCH,
    GLGE_KEY_APP_CTRL_HOME,
    GLGE_KEY_APP_CTRL_BACK,
    GLGE_KEY_APP_CTRL_FORWARD,
    GLGE_KEY_APP_CTRL_STOP,
    GLGE_KEY_APP_CTRL_REFRESH,
    GLGE_KEY_APP_CTRL_BOOKMARKS,

    //special symbols

    GLGE_KEY_EXCLAIM,
    GLGE_KEY_HASH,
    GLGE_KEY_DOLLAR,
    GLGE_KEY_PERCENT,
    GLGE_KEY_AMPERSAND,
    GLGE_KEY_APOSTROPHE,
    GLGE_KEY_LEFT_PRENTICES,
    GLGE_KEY_RIGHT_PRENTICES,
    GLGE_KEY_ASTERISK,
    GLGE_KEY_PLUS,
    GLGE_KEY_COMMA,
    GLGE_KEY_MINUS,
    GLGE_KEY_PERIOD,
    GLGE_KEY_SLASH,
    GLGE_KEY_COLON,
    GLGE_KEY_SEMICOLON,
    GLGE_KEY_LESS,
    GLGE_KEY_EQUALS,
    GLGE_KEY_GREATER,
    GLGE_KEY_QUESTION,
    GLGE_KEY_AT,
    GLGE_KEY_LEFT_BRACKET,
    GLGE_KEY_BACKSLASH,
    GLGE_KEY_RIGHT_BRACKET,
    GLGE_KEY_CARET,
    GLGE_KEY_UNDERSCORE,
    GLGE_KEY_GRAVE,
    GLGE_KEY_LEFT_BRACE,
    GLGE_KEY_PIPE,
    GLGE_KEY_RIGHT_BRACE,
    GLGE_KEY_TILDE,
    GLGE_KEY_PLUS_MINUS,
    GLGE_KEY_PRINT_SCREEN,
    GLGE_KEY_SCROLL_LOCK,
    GLGE_KEY_PAUSE,
    GLGE_KEY_INSERT,
    GLGE_KEY_HOME,
    GLGE_KEY_PAGE_UP,
    GLGE_KEY_PAGE_DOWN,
    GLGE_KEY_END,
    GLGE_KEY_RIGHT,
    GLGE_KEY_LEFT,
    GLGE_KEY_DOWN,
    GLGE_KEY_UP,
    GLGE_KEY_NUM_LOCK_CLEAR,
    GLGE_KEY_APPLICATION,
    GLGE_KEY_POWER,
    GLGE_KEY_EXECUTE,
    GLGE_KEY_HELP,
    GLGE_KEY_MENU,
    GLGE_KEY_SELECT,
    GLGE_KEY_STOP,
    GLGE_KEY_AGAIN,
    GLGE_KEY_UNDO,
    GLGE_KEY_CUT,
    GLGE_KEY_COPY,
    GLGE_KEY_PASTE,
    GLGE_KEY_FIND,
    GLGE_KEY_MUTE,
    GLGE_KEY_VOLUME_UP,
    GLGE_KEY_VOLUME_DOWN,
    GLGE_KEY_ALT_ERASE,
    GLGE_KEY_SYS_REQ,
    GLGE_KEY_CLEAR,
    GLGE_KEY_PRIOR,
    GLGE_KEY_RETURN,
    GLGE_KEY_SEPARATOR,
    GLGE_KEY_OUT,
    GLGE_KEY_OPER,
    GLGE_KEY_CLEAR_AGAIN,
    GLGE_KEY_CRSEL,
    GLGE_KEY_EXSEL,
    GLGE_KEY_THOUSANDS_SEPARATOR,
    GLGE_KEY_DECIMAL_SEPARATOR,
    GLGE_KEY_CURRENCY_UNIT,
    GLGE_KEY_CURRENCY_SUBUNIT,

    //numpad

    GLGE_KEY_NUMPAD_DIVIDE,
    GLGE_KEY_NUMPAD_MULTIPLY,
    GLGE_KEY_NUMPAD_MINUS,
    GLGE_KEY_NUMPAD_PLUS,
    GLGE_KEY_NUMPAD_ENTER,
    GLGE_KEY_NUMPAD_0,
    GLGE_KEY_NUMPAD_1,
    GLGE_KEY_NUMPAD_2,
    GLGE_KEY_NUMPAD_3,
    GLGE_KEY_NUMPAD_4,
    GLGE_KEY_NUMPAD_5,
    GLGE_KEY_NUMPAD_6,
    GLGE_KEY_NUMPAD_7,
    GLGE_KEY_NUMPAD_8,
    GLGE_KEY_NUMPAD_9,
    GLGE_KEY_NUMPAD_PERIOD,
    GLGE_KEY_NUMPAD_EQUALS,
    GLGE_KEY_NUMPAD_COMMA,
    GLGE_KEY_NUMPAD_EQUALS_AS_400,
    GLGE_KEY_NUMPAD_00,
    GLGE_KEY_NUMPAD_000,
    GLGE_KEY_NUMPAD_LEFT_PRENTICES,
    GLGE_KEY_NUMPAD_RIGHT_PRENTICES,
    GLGE_KEY_NUMPAD_LEFT_BRACE,
    GLGE_KEY_NUMPAD_RIGHT_BRACE,
    GLGE_KEY_NUMPAD_TAB,
    GLGE_KEY_NUMPAD_BACKSPACE,
    GLGE_KEY_NUMPAD_A,
    GLGE_KEY_NUMPAD_B,
    GLGE_KEY_NUMPAD_C,
    GLGE_KEY_NUMPAD_D,
    GLGE_KEY_NUMPAD_E,
    GLGE_KEY_NUMPAD_F,
    GLGE_KEY_NUMPAD_XOR,
    GLGE_KEY_NUMPAD_POWER,
    GLGE_KEY_NUMPAD_PERCENT,
    GLGE_KEY_NUMPAD_LESS,
    GLGE_KEY_NUMPAD_GREATER,
    GLGE_KEY_NUMPAD_AMPERSAND,
    GLGE_KEY_NUMPAD_DBL_AMPERSAND,
    GLGE_KEY_NUMPAD_VERTICAL_BAR,
    GLGE_KEY_NUMPAD_DBL_VERTICAL_BAR,
    GLGE_KEY_NUMPAD_COLON,
    GLGE_KEY_NUMPAD_HASH,
    GLGE_KEY_NUMPAD_SPACE,
    GLGE_KEY_NUMPAD_AT,
    GLGE_KEY_NUMPAD_EXCLAM,
    GLGE_KEY_NUMPAD_MEM_STORE,
    GLGE_KEY_NUMPAD_MEM_RECALL,
    GLGE_KEY_NUMPAD_MEM_CLEAR,
    GLGE_KEY_NUMPAD_MEM_ADD,
    GLGE_KEY_NUMPAD_MEM_SUBTRACT,
    GLGE_KEY_NUMPAD_MEM_MULTIPLY,
    GLGE_KEY_NUMPAD_MEM_DIVIDE,
    GLGE_KEY_NUMPAD_PLUS_MINUS,
    GLGE_KEY_NUMPAD_CLEAR,
    GLGE_KEY_NUMPAD_CLEAR_ENTRY,
    GLGE_KEY_NUMPAD_BINARY,
    GLGE_KEY_NUMPAD_OCTAL,
    GLGE_KEY_NUMPAD_DECIMAL,
    GLGE_KEY_NUMPAD_HEXADECIMAL,

    //F keys

    GLGE_KEY_F1,
    GLGE_KEY_F2,
    GLGE_KEY_F3,
    GLGE_KEY_F4,
    GLGE_KEY_F5,
    GLGE_KEY_F6,
    GLGE_KEY_F7,
    GLGE_KEY_F8,
    GLGE_KEY_F9,
    GLGE_KEY_F10,
    GLGE_KEY_F11,
    GLGE_KEY_F12,
    GLGE_KEY_F13,
    GLGE_KEY_F14,
    GLGE_KEY_F15,
    GLGE_KEY_F16,
    GLGE_KEY_F17,
    GLGE_KEY_F18,
    GLGE_KEY_F19,
    GLGE_KEY_F20,
    GLGE_KEY_F21,
    GLGE_KEY_F22,
    GLGE_KEY_F23,
    GLGE_KEY_F24,

    //numbers

    GLGE_KEY_0,
    GLGE_KEY_1,
    GLGE_KEY_2,
    GLGE_KEY_3,
    GLGE_KEY_4,
    GLGE_KEY_5,
    GLGE_KEY_6,
    GLGE_KEY_7,
    GLGE_KEY_8,
    GLGE_KEY_9,

    //max enum value

    GLGE_KEY_ENUM_MAX = 0xFF
} Key;

//define a type that holds one bit for every key possible
//1 : the key is signaled
//0 : the key is not signaled
typedef uint64_t Keys[4];

/**
 * @brief check if a key in a specific selection of keys is signaled
 * 
 * @param key the key to check if it is signaled
 * @param keys the key storage to check in
 * @return true : the key is signaled
 * @return false : the key is not signaled
 */
bool key_isSignaled(Key key, const Keys* keys) noexcept;

/**
 * @brief set if a key is signaled or not
 * 
 * @param key the key to update
 * @param state the state to set the key to
 * @param keys the key storage to update the key for
 */
void key_setSignaled(Key key, bool state, Keys* keys) noexcept;

//everything is available in C
#if __cplusplus
}
#endif

//for C++ add a layer that handles key presses
#if __cplusplus

//add the layer base class
#include "../../../GLGE_Core/Layers/LayerBase.hpp"

//define an event type tp use for key down events
inline constexpr const EventType GLGE_KEY_EVENT_KEY_DOWN{"GLGE_GRAPHIC", 0xF1};
//define an event type to use for key up events
inline constexpr const EventType GLGE_KEY_EVENT_KEY_UP{"GLGE_GRAPHIC", 0xF2};

/**
 * @brief store a class that handles key presses
 */
class KeyLayer : public LayerBase {
public:

    /**
     * @brief Construct a new Key Layer
     * 
     * @param the name for the key layer
     */
    KeyLayer(const char* name = "MAIN_KEYBOARD")
     : LayerBase(LayerType("GLGE_GRAPHIC", name), staticEventHandle, this, false)
    {}

    /**
     * @brief tick the key layer
     */
    virtual void onUpdate() noexcept override;

    /**
     * @brief Get the currently pressed keys
     * 
     * @return const Keys& a constant reference to all currently pressed keys
     */
    inline const Keys& getCurrentKeys() const noexcept {return m_pressed;}

    /**
     * @brief Get the keys that where pressed this tick
     * 
     * @return const Keys& a constant reference to the keys that were pressed this tick
     */
    inline const Keys& getPressedKeys() const noexcept {return m_enabled;}

    /**
     * @brief Get the keys that where released this tick
     * 
     * @return const Keys& a constant reference to the keys that where released this tick
     */
    inline const Keys& getReleasedKeys() const noexcept {return m_released;}

    /**
     * @brief Get the keys that where repeatedly pressed this tick
     * 
     * @return const Keys& a constant reference to the repeated keys
     */
    inline const Keys& getRepeatedKeys() const noexcept {return m_repeated;}

protected:

    //store if the key is pressed
    Keys m_pressed;
    //store if the key was enabled
    Keys m_enabled;
    //store if the key was released
    Keys m_released;
    //store if the key was repeated
    Keys m_repeated;

    /**
     * @brief handle an event
     * 
     * @param event the event to handle
     */
    void handleEvent(const Event* event);

    /**
     * @brief 
     * 
     * @param event 
     * @param layer 
     */
    inline static void staticEventHandle(const Event* event, void* layer) noexcept 
    {((KeyLayer*)layer)->handleEvent(event);}

};

#endif

#endif