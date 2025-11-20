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

Buffer* glge_Graphic_GetMeshBuffer() {return GLGE::Graphic::Backend::INSTANCE.getInstance()->getMeshBuffer();}

void glge_Graphic_MainTick()
{
    //simply update the backend instance
    GLGE::Graphic::Backend::INSTANCE.update();
}

const Keys* glge_Graphic_GetCurrentKeys()
{return &GLGE::Graphic::Backend::INSTANCE.getKeyLayer().getCurrentKeys();}

const Keys* glge_Graphic_GetPressedKeys()
{return &GLGE::Graphic::Backend::INSTANCE.getKeyLayer().getPressedKeys();}

const Keys* glge_Graphic_GetReleasedKeys()
{return &GLGE::Graphic::Backend::INSTANCE.getKeyLayer().getReleasedKeys();}

const Keys* glge_Graphic_GetRepeatedKeys()
{return &GLGE::Graphic::Backend::INSTANCE.getKeyLayer().getRepeatedKeys();}

uint64_t glge_Graphic_GetMiceCount()
{return GLGE::Graphic::Backend::INSTANCE.getMouseLayer().getMiceCount();}

const Mouse* glge_Graphic_GetMouse(uint32_t id)
{return &GLGE::Graphic::Backend::INSTANCE.getMouseLayer().getMouse(id);}

const Mouse* glge_Graphic_GetGlobalMouse()
{return &GLGE::Graphic::Backend::INSTANCE.getGlobalMouse();}

const Mouse* glge_Graphic_GetRelativeMouse()
{return &GLGE::Graphic::Backend::INSTANCE.getRelativeMouse();}

LayerStack* glge_Graphic_AccessEventStack()
{return &GLGE::Graphic::Backend::INSTANCE.eventStack();}

bool glge_Graphic_IsAPISupported(InstanceAPI api)
{return glge_graphic_IsAPISupported(api);}

InstanceAPI glge_Graphic_GetCurrentAPI()
{return GLGE::Graphic::Backend::INSTANCE.getAPI();}

bool glge_Graphic_SelectAPI(InstanceAPI api)
{return GLGE::Graphic::Backend::INSTANCE.setAPI(api);}
