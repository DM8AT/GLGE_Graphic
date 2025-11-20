/**
 * @file Mouse.cpp
 * @author DM8AT
 * @brief implement the functionality for mice
 * @version 0.1
 * @date 2025-11-19
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//add SDL3
#include "SDL3/SDL.h"
//add the mice API
#include "Mouse.h"
//add algorithms
#include <algorithm>

bool mouse_isPressed(uint8_t button, const MouseButtons* mouse) {
    //get the index of the element to store to
    uint8_t el = button / 64;
    //get the index to store to
    uint8_t id = button % 64;

    //return the requested state
    return ((*mouse)[el] >> id) & 0b1;
}

void mouse_setState(uint8_t button, bool state, MouseButtons* mouse) {
    //get the index of the element to store to
    uint8_t el = button / 64;
    //get the index to store to
    uint8_t id = button % 64;
    
    //pre-compute the mask
    uint64_t mask = (uint64_t)1 << id;

    //store the state
    if (state) {
        (*mouse)[el] |= mask;
    } else {
        (*mouse)[el] &= ~mask;
    }
}

void MouseLayer::init() noexcept {
    //add all mice
    int32_t miceCount = 0;
    SDL_MouseID* ids = SDL_GetMice(&miceCount);
    //iterate over all mice and add them
    for (int32_t i = 0; i < miceCount; ++i) 
    {addMouse(ids[i]);}
}

void MouseLayer::onUpdate() noexcept {
    //iterate over all mice and remove the per-tick update
    for (size_t i = 0; i < m_mice.size(); ++i) {
        //remove the pressed state
        m_mice[i].pressed[0] = 0;
        m_mice[i].pressed[1] = 0;
        m_mice[i].pressed[2] = 0;
        m_mice[i].pressed[3] = 0;
        //remove the released state
        m_mice[i].released[0] = 0;
        m_mice[i].released[1] = 0;
        m_mice[i].released[2] = 0;
        m_mice[i].released[3] = 0;
    }
}

void MouseLayer::handleEvent(const Event* event) noexcept {
    //check for different events

    //check for a mouse move movement
    if (*event == Event(GLGE_MOUSE_EVENT_MOUSE_MOVE, EventData{0, nullptr})) {
        //get the data
        GLGE_MOUSE_EVENT_DATA* data = (GLGE_MOUSE_EVENT_DATA*)event->data.inPlace;
        //get the mouse to update
        Mouse& mouse = m_mice[m_mapping[data->mouse]];
        //store the new position
        mouse.pixelPos = data->data.pos;
    }
    //check for a mouse button down event
    if (*event == Event(GLGE_MOUSE_EVENT_MOUSE_DOWN, EventData{0, nullptr})) {
        //get the data
        GLGE_MOUSE_EVENT_DATA* data = (GLGE_MOUSE_EVENT_DATA*)event->data.inPlace;
        //get the mouse to update
        Mouse& mouse = m_mice[m_mapping[data->mouse]];
        //store the new button state
        mouse_setState(data->data.button, true, &mouse.current);
        mouse_setState(data->data.button, true, &mouse.pressed);
    }
    //check for a mouse button up event
    if (*event == Event(GLGE_MOUSE_EVENT_MOUSE_UP, EventData{0, nullptr})) {
        //get the data
        GLGE_MOUSE_EVENT_DATA* data = (GLGE_MOUSE_EVENT_DATA*)event->data.inPlace;
        //get the mouse to update
        Mouse& mouse = m_mice[m_mapping[data->mouse]];
        //store the new button state
        mouse_setState(data->data.button, false, &mouse.current);
        mouse_setState(data->data.button, true, &mouse.released);
    }
}

void MouseLayer::addMouse(uint32_t mouseID) noexcept
{
    //check if the free list has entries
    if (m_freeList.size()) {
        //there are entries
        //re-use the an entry
        uint32_t idx = m_freeList.back();
        m_freeList.pop_back();
        //add the entry
        addMouseEntry(mouseID, idx);
    } else {
        //just add a new element
        uint32_t idx = m_mapping.size();
        m_mice.push_back(Mouse{ 0 });
        //add the entry
        addMouseEntry(mouseID, idx);
    }
}

void MouseLayer::addMouseEntry(uint32_t mouseID, uint32_t glgeId) noexcept
{
    //store the mapping
    m_mapping.insert_or_assign(mouseID, glgeId);
    //get the name
    const char* name = SDL_GetMouseNameForID(mouseID);
    //write the new mouse data
    m_mice[glgeId] = Mouse{
        .name = name,
        .pixelPos = ivec2(0,0),
        .current = { 0 },
        .pressed = { 0 },
        .released = { 0 }
    };
    //store the name mapping
    auto it = m_nameMap.find(name);
    //if it was found, add the mapping to the list. Else, add a new list
    if (it == m_nameMap.end()) 
    {m_nameMap.insert_or_assign(name, std::vector<uint32_t>{mouseID});}
    else 
    {it->second.push_back(mouseID);}
}

void MouseLayer::removeMouse(uint32_t mouseID) noexcept
{
    //quarry the actual ID of the mouse to remove
    auto it_m = m_mapping.find(mouseID);
    //add the index to the free list
    m_freeList.push_back(it_m->second);
    //remove the element
    for (auto it = m_nameMap.begin(); it != m_nameMap.end(); ) {
        auto it2 = std::find(it->second.begin(), it->second.end(), mouseID);
        if (it2 == it->second.end()) {
            ++it;
        } else {
            it->second.erase(it2);
            //if the element is empty, remove it
            if (it->second.size() == 0) 
            {it = m_nameMap.erase(it);}
        }
    }
}