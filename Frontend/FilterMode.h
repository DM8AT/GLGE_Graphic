/**
 * @file FilterMode.h
 * @author DM8AT
 * @brief define an enum to use to define filter modes
 * @version 0.1
 * @date 2025-11-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//header guard
#ifndef _GLGE_GRAPHIC_FRONTEND_FILTER_MODE_
#define _GLGE_GRAPHIC_FRONTEND_FILTER_MODE_

/**
 * @brief define an enum that is used to select the filter mode of a texture
 */
typedef enum e_FilterMode 
{
    //the texture will be sampled using bi-linear interpolation between the closest texels
    GLGE_FILTER_MODE_LINEAR = 0,
    //the value of the nearest texel will be used
    GLGE_FILTER_MODE_NEAREST = 1
} FilterMode;

#endif