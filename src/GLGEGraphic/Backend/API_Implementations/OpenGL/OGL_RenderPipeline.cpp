/**
 * @file OGL_RenderPipeline.cpp
 * @author DM8AT
 * @brief implement the OpenGL render pipeline
 * @version 0.1
 * @date 2025-10-19
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//add the render pipeline
#include "OGL_RenderPipeline.h"

//add SDL for easy access
#include "SDL3/SDL.h"
//add the window
#include "../../../Frontend/Window/Window.h"
#include "OGL_Window.h"

//use the namespace
using namespace GLGE::Graphic::Backend::OGL;

void GLGE::Graphic::Backend::OGL::RenderPipeline::record() noexcept
{
    //clean the command buffer
    m_cmdBuff.clear();
    //just iterate over all stages and play them
    for (auto it = m_pipeline->getStages().begin(); it != m_pipeline->getStages().end(); ++it)
    {execute(it->second);}

    //recording done
    m_cmdBuff.markRecorded();
}

void GLGE::Graphic::Backend::OGL::RenderPipeline::execute(const RenderPipelineStage& stage) noexcept
{
    //switch over the stage type to do the correct stuff
    switch (stage.type)
    {
    case GLGE_RENDER_PIPELINE_STAGE_CUSTOM:
        executeStage_Custom(stage.data.customStage);
        break;
    
    case GLGE_RENDER_PIPELINE_STAGE_START_WINDOW_FRAME:
        executeStage_WindowFrameStart(stage.data.windowFrameStart);
        break;

    case GLGE_RENDER_PIPELINE_STAGE_END_WINDOW_FRAME:
        executeStage_WindowFrameEnd(stage.data.windowFrameEnd);
        break;

    default:
        GLGE_DEBUG_ABORT("Unknown render pipeline stage");
        break;
    }
}

void GLGE::Graphic::Backend::OGL::RenderPipeline::executeStage_Custom(const RenderPipelineStageData::CustomStage& stage) noexcept
{
    //add the custom stage
    m_cmdBuff.record<Command_Custom>(stage.custom_func, stage.userData);
}

void GLGE::Graphic::Backend::OGL::RenderPipeline::executeStage_WindowFrameStart(const RenderPipelineStageData::WindowFrameStart& stage) noexcept
{
    //simply start the window using the built in function
    ((OGL::Window*)((::Window*)stage.window)->getAPI())->beginFrame(&m_cmdBuff);
}

void GLGE::Graphic::Backend::OGL::RenderPipeline::executeStage_WindowFrameEnd(const RenderPipelineStageData::WindowFrameEnd& stage) noexcept
{
    //simply end the window frame using the built in function
    ((OGL::Window*)((::Window*)stage.window)->getAPI())->endFrame(&m_cmdBuff);
}