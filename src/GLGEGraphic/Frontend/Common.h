/**
 * @file Common.h
 * @author DM8AT
 * @brief define some common functionality that is needed in the frontend but does not fit to a class
 * @version 0.1
 * @date 2025-10-15
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_GRAPHIC_FRONTEND_COMMON_
#define _GLGE_GRAPHIC_FRONTEND_COMMON_

//add buffers
#include "Buffer.h"
//add keys
#include "Keys.h"
//add mice
#include "Mouse.h"
//add the layer system
#include "../../GLGE_Core/Layers/Layers.h"

//currently only available in C++
#if __cplusplus

/**
 * @brief get the global transform buffer of all objects
 * 
 * @return Buffer* a pointer to the main transform buffer
 */
::Buffer* glge_Graphic_GetTransformBuffer();

/**
 * @brief get the global mesh buffer
 * 
 * @return ::Buffer* a pointer to the global mesh buffer
 */
::Buffer* glge_Graphic_GetMeshBuffer();

#endif

//make everything available for C
#if __cplusplus
extern "C" {
#endif

//define a C/C++ enum for the type of Graphic API
typedef enum e_InstanceAPI {
    //automatically select the best API available
    GLGE_GRAPHIC_INSTANCE_API_AUTO = 0,
    //use OpenGL (uses OpenGL 4.6)
    GLGE_GRAPHIC_INSTANCE_API_OPEN_GL,
    //use Vulkan
    GLGE_GRAPHIC_INSTANCE_API_VULKAN,
    //use directX 11
    GLGE_GRAPHIC_INSTANCE_API_DIRECT_X11,
    //use directX 12
    GLGE_GRAPHIC_INSTANCE_API_DIRECT_X12
} InstanceAPI;

/**
 * @brief main tick for the graphic core
 */
void glge_Graphic_MainTick();

/**
 * @brief get the currently pressed keys
 * 
 * @return const Keys* a constant pointer to the currently pressed keys
 */
const Keys* glge_Graphic_GetCurrentKeys();

/**
 * @brief get the keys that where pressed this tick
 * 
 * @return const Keys* a constant pointer to the keys that where pressed this tick
 */
const Keys* glge_Graphic_GetPressedKeys();

/**
 * @brief get the keys that where released this tick
 * 
 * @return const Keys* a constant pointer to the keys that where released this tick
 */
const Keys* glge_Graphic_GetReleasedKeys();

/**
 * @brief get the keys that where repeated this tick
 * 
 * That means the key is allready pressed down but was pressed down again without being pulled up first
 * 
 * @return const Keys* a constant pointer to the repeated keys
 */
const Keys* glge_Graphic_GetRepeatedKeys();

/**
 * @brief get the amount of mice currently registered
 * 
 * @return uint64_t the amount of currently registered files
 */
uint64_t glge_Graphic_GetMiceCount();

/**
 * @brief get a specific mouse using an ID
 * 
 * @param id the ID to quarry the mouse from
 * @return const Mouse* a pointer to the mouse
 */
const Mouse* glge_Graphic_GetMouse(uint32_t id);

/**
 * @brief get the global mouse
 * 
 * @return const Mouse* a constant pointer to the global mouse
 */
const Mouse* glge_Graphic_GetGlobalMouse();

/**
 * @brief get a relative mouse (this should be used if a window is in relative mouse mode)
 * 
 * @return const Mouse* a constant pointer to the relative mouse
 */
const Mouse* glge_Graphic_GetRelativeMouse();

/**
 * @brief get a pointer to the global main event stack
 * 
 * @return LayerStack* a pointer to the main event stack
 */
LayerStack* glge_Graphic_AccessEventStack();

/**
 * @brief check if a specific graphic API is supported
 * 
 * @param api the API to check
 * @return true : the API is supported
 * @return false : the API is not supported
 */
bool glge_Graphic_IsAPISupported(InstanceAPI api);

/**
 * @brief get the API that is currently selected
 * 
 * @return InstanceAPI the currently selected API
 */
InstanceAPI glge_Graphic_GetCurrentAPI();

/**
 * @brief set the current API
 * 
 * @param api the new API
 * @return true : successfully selected the API
 * @return false : failed to select the API, the main instance was allready initialized
 */
bool glge_Graphic_SelectAPI(InstanceAPI api);

#if __cplusplus
}
#endif

#endif