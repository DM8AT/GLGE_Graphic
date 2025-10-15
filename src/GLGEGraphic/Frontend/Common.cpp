/**
 * @file Common.cpp
 * @author DM8AT
 * @brief implement the common functions
 * @version 0.1
 * @date 2025-10-15
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//include the common stuff
#include "Common.h"

//include the backend
#include "../Backend/Backend.h"


void glge_Graphic_MainTick()
{
    //simply update the backend instance
    GLGE::Graphic::Backend::INSTANCE.update();
}