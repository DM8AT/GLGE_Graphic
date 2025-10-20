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

//add SDL for easy access
#include "SDL3/SDL.h"
//add the render pipeline
#include "OGL_RenderPipeline.h"

//add the window
#include "../../../Frontend/Window/Window.h"
#include "OGL_Window.h"

//use the namespace
using namespace GLGE::Graphic::Backend::OGL;

void GLGE::Graphic::Backend::OGL::RenderPipeline::record() noexcept
{
    //clean the command buffer
    m_cmdBuff.clear();
    //cash the clear color
    m_clearColor = (m_pipeline->getWindow()) ? m_pipeline->getWindow()->getClearColor() : 0;
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


void GLGE::Graphic::Backend::OGL::RenderPipeline::play() noexcept
{
    //change the path of execution depending on if a window exists
    if (m_pipeline->getWindow()) {
        //first, prepare the window
        ((OGL::Window*)m_pipeline->getWindow()->getAPI())->beginFrame();
        //then play back the command buffer if the window is not minimized
        if (!m_pipeline->getWindow()->getSettings().minimized) {
            //first, clear the window
            ((OGL::Window*)m_pipeline->getWindow()->getAPI())->clearWindow(m_clearColor);
            //just play back the command buffer
            m_cmdBuff.play();
            //finally end the tick
            ((OGL::Window*)m_pipeline->getWindow()->getAPI())->endFrame();
        }
    } else {
        //if no window exists, just run the pipeline
        m_cmdBuff.play();
    }
}