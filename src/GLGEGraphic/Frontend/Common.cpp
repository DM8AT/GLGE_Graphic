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

Buffer* glge_Graphic_GetTransformBuffer() {return GLGE::Graphic::Backend::RenderObjectSystem::getTransformBuffer();}

void glge_Graphic_MainTick()
{
    //simply update the backend instance
    GLGE::Graphic::Backend::INSTANCE.update();
}

bool glge_Graphic_IsAPISupported(InstanceAPI api)
{return glge_graphic_IsAPISupported(api);}

InstanceAPI glge_Graphic_GetCurrentAPI()
{return GLGE::Graphic::Backend::INSTANCE.getAPI();}

bool glge_Graphic_SelectAPI(InstanceAPI api)
{return GLGE::Graphic::Backend::INSTANCE.setAPI(api);}
