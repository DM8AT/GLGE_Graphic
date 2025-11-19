/**
 * @file Keys.cpp
 * @author DM8AT
 * @brief implement the core functionalities for the key system
 * @version 0.1
 * @date 2025-11-18
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//add the key API
#include "Keys.h"
//add memset
#include <cstring>

bool key_isSignaled(Key key, const Keys* keys) noexcept
{
    //check in which bit section the key is
    uint8_t i_key = (uint8_t)key;
    //compute which uint64_t to use
    uint8_t w = i_key / 64;
    //compute the offset
    uint8_t o = i_key - (i_key * 64);

    //return the value
    return ((*keys)[w] >> o) & 0b1;
}

void key_setSignaled(Key key, bool state, Keys* keys) noexcept
{
    //check in which bit section the key is
    uint8_t i_key = (uint8_t)key;
    //compute which uint64_t to use
    uint8_t w = i_key / 64;
    //compute the offset
    uint8_t o = i_key - (i_key * 64);

    //pre-compute the mask
    uint64_t mask = (uint64_t)1 << o;

    //set the state correctly
    if (state) {
        (*keys)[w] |= mask;
    } else {
        (*keys)[w] &= ~mask;
    }
}

void KeyLayer::onUpdate() noexcept
{
    //reset the enabled / disabled / released registers
    // enabled
    m_enabled[0] = 0;
    m_enabled[1] = 0;
    m_enabled[2] = 0;
    m_enabled[3] = 0;
    // released
    m_released[0] = 0;
    m_released[1] = 0;
    m_released[2] = 0;
    m_released[3] = 0;
    // repeated
    m_repeated[0] = 0;
    m_repeated[1] = 0;
    m_repeated[2] = 0;
    m_repeated[3] = 0;
}

void KeyLayer::handleEvent(const Event* event)
{
    //check if this is a key down event
    if (*event == Event(GLGE_KEY_EVENT_KEY_DOWN, EventData(0, nullptr))) {
        //extract the needed data
        Key key = (Key)event->data.inPlace[0];
        //check for a repeat
        key_setSignaled(key, key_isSignaled(key, &m_pressed), &m_repeated);
        //set the state for the key
        key_setSignaled(key, !key_isSignaled(key, &m_pressed), &m_enabled);
        key_setSignaled(key, true, &m_pressed);
    }
    else if (*event == Event(GLGE_KEY_EVENT_KEY_UP, EventData(0, nullptr))) {
        //extract the needed data
        Key key = (Key)event->data.inPlace[0];
        //set the state for the key
        key_setSignaled(key, false, &m_pressed);
        key_setSignaled(key, true, &m_released);
    }
}