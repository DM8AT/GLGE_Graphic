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

//currently only available in C++
#if __cplusplus

/**
 * @brief get the global transform buffer of all objects
 * 
 * @return Buffer* a pointer to the main transform buffer
 */
Buffer* glge_Graphic_GetTransformBuffer();

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