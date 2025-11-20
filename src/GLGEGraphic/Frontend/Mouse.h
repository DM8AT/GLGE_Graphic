/**
 * @file Mouse.h
 * @author DM8AT
 * @brief define an interface for the mouse
 * @version 0.1
 * @date 2025-11-19
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//header guard
#ifndef _GLGE_GRAPHIC_FRONTEND_MOUSE_
#define _GLGE_GRAPHIC_FRONTEND_MOUSE_

//add types
#include "../../GLGE_Core/Types.h"

//add vector types
#include "../../GLGE_Math/GLGEMath.h"

//everything is available in C
#if __cplusplus
extern "C" {
#endif

//some defines to make mouse button indices clearer

//the left mouse button's index
#define GLGE_MOUSE_LEFT 1
//the right mouse button's index
#define GLGE_MOUSE_RIGHT 3
//the middle mouse button's index
#define GLGE_MOUSE_MIDDLE 2

//define a type that stores the state of all mouse buttons
typedef uint64_t MouseButtons[4];
//store a mapping from a mouse button ID to a click count
typedef struct s_MouseClick {uint8_t button; uint8_t clickCount;} MouseClick;

//store the whole state of a mouse
typedef struct s_Mouse {
    //store the name of the mouse
    const char* name;
    //store the pixel position of the mouse
    ivec2 pixelPos;
    //store the currently pressed mouse buttons
    MouseButtons current;
    //store the mouse buttons that where pressed this tick
    MouseButtons pressed;
    //store the mouse buttons that where released this tick
    MouseButtons released;
} Mouse;

/**
 * @brief check if a specific mouse button is pressed
 * 
 * @param button the index of the button to check
 * @param mouse a constant pointer to the mouse button storage to quarry the data from
 * @return true : the button is pressed
 * @return false : the button is not pressed
 */
bool mouse_isPressed(uint8_t button, const MouseButtons* mouse);

/**
 * @brief set a specific button to a specific state
 * 
 * @param button the index of the button to update
 * @param state the state to set the button to
 * @param mouse a pointer to the mouse button storage to store the data in
 */
void mouse_setState(uint8_t button, bool state, MouseButtons* mouse);

//everything is available in C
#if __cplusplus
}
#endif

//the mouse layer is only available in C++
#if __cplusplus

//add the layer system
#include "../../GLGE_Core/Layers/LayerBase.hpp"

//add a deque to create a mapping from SDL id to mice with constant pointers
#include <deque>
//add a hash map to map from SDL mouse ids to GLGE ids
#include <unordered_map>
//add a vector to add as a free list
#include <vector>
//for static strings
#include <string_view>
//add a mutex for thread safety
#include <mutex>
//add vectors
#include <vector>

//define some constant event types

//define an event type to notify of mice movement
inline constexpr const EventType GLGE_MOUSE_EVENT_MOUSE_MOVE{"GLGE_GRAPHIC", 0xFFF1};
//define an event type to notify of mice button down events
inline constexpr const EventType GLGE_MOUSE_EVENT_MOUSE_DOWN{"GLGE_GRAPHIC", 0xFFF2};
//define an event type to notify of mice button up events
inline constexpr const EventType GLGE_MOUSE_EVENT_MOUSE_UP{"GLGE_GRAPHIC", 0xFFF3};
//define an event type to notify of mice addition
inline constexpr const EventType GLGE_MOUSE_EVENT_MOUSE_PLUG_IN{"GLGE_GRAPHIC", 0xFFF4};
//define an event type to notify of mice removenet
inline constexpr const EventType GLGE_MOUSE_EVENT_MOUSE_PLUG_OUT{"GLGE_GRAPHIC", 0xFFF5};

/**
 * @brief store a structure for mouse events
 */
struct GLGE_MOUSE_EVENT_DATA {
    //store the SDL ID of the mouse
    uint32_t mouse;
    //store a union of the data
    union Data {
        //store the pixel position for movement
        ivec2 pos;
        //store the ID of the button that changed
        uint8_t button;
        //the add / unplug event doesn't need data
    } data;
};

/**
 * @brief a class that manages mouse events
 */
class MouseLayer : public LayerBase {
public:

    /**
     * @brief Construct a new Mouse Layer
     * 
     * @param name an optional name for the mouse layer
     */
    MouseLayer(const char* name = "MOUSE_MAIN")
     : LayerBase(LayerType("GLGE_GRAPHIC", name), staticEventHandle, this) 
    {}

    /**
     * @brief initialize the mouse layer
     */
    void init() noexcept;

    /**
     * @brief update the mouse layer
     */
    virtual void onUpdate() noexcept override;

    /**
     * @brief Get the amount of registered mice
     * 
     * @return uint64_t the amount of currently available elements
     */
    inline uint64_t getMiceCount() const noexcept {return m_mapping.size();}

    /**
     * @brief Get a mouse using a mouse index
     * 
     * @param id the index of the mouse to quarry
     * @return const Mouse& a constant reference to the mouse
     */
    inline const Mouse& getMouse(uint32_t id) const noexcept {return m_mice[id];}

protected:

    //store a mapping of an uint32_t to a mice where the pointer to the mice are constant
    std::deque<Mouse> m_mice;
    //store a list of free mouse slots
    std::vector<uint32_t> m_freeList;
    //store a mapping from SDL IDs to GLGE mice ids
    std::unordered_map<uint32_t, uint32_t> m_mapping;
    //store a mapping from mouse names to GLGE indices
    std::unordered_map<std::string_view, std::vector<uint32_t>> m_nameMap;

    /**
     * @brief add a new mouse to the mouse layer
     * 
     * @param mouseID the SDL mouse id of the new mouse
     */
    void addMouse(uint32_t mouseID) noexcept;

    /**
     * @brief add a mouse entry
     * 
     * @param mouseID the SDL index of the mouse
     * @param glgeId the GLGE index for the mouse
     */
    void addMouseEntry(uint32_t mouseID, uint32_t glgeId) noexcept;

    /**
     * @brief remove a specific size
     * 
     * @param mouseID the SDL id of the mouse to remove
     */
    void removeMouse(uint32_t mouseID) noexcept;

    /**
     * @brief handle a single event
     * 
     * @param event the event to handle
     */
    void handleEvent(const Event* event) noexcept;

    /**
     * @brief a static function to handle an event
     * 
     * @param event a constant pointer to the event to handle
     * @param mouseLayer a pointer to the mouse layer that should handle the event
     */
    inline static void staticEventHandle(const Event* event, void* mouseLayer) noexcept
    {((MouseLayer*)mouseLayer)->handleEvent(event);}

};

#endif

#endif