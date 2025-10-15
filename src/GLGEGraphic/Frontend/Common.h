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

//make everything available for C
#if __cplusplus
extern "C" {
#endif

/**
 * @brief main tick for the graphic core
 */
void glge_Graphic_MainTick();

#if __cplusplus
}
#endif

#endif