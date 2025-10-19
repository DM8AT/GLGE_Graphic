/**
 * @file RenderPipeline.cpp
 * @author DM8AT
 * @brief implement the functions for the render pipeline frontend
 * @version 0.1
 * @date 2025-10-18
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//include the render pipeline
#include "RenderPipeline.h"
//add debugging
#include "../../GLGE_BG/Debugging/Logging/__BG_SimpleDebug.h"

//add instances
#include "../Backend/Instance.h"
//add all API implementations
#include "../Backend/API_Implementations/AllImplementations.h"

RenderPipeline::RenderPipeline(const std::map<String, RenderPipelineStage> stages)
 : //just copy the stages over
   m_stages(stages)
{
    //add the backend API
    initializeAPI();
}

RenderPipeline::RenderPipeline(const RenderPipelineStageNamed* namedStages, size_t namedStageCount)
{
    //iterate over all stages and unpack the named stage structure
    for (size_t i = 0; i < namedStageCount; ++i) 
    {
        //for debugging purpose check if the name allready exists
        GLGE_DEBUG_ASSERT("Adding a stage to an allready existing name - overriding the original stage", containsStage(namedStages[i].name));
        m_stages.insert_or_assign(namedStages[i].name, namedStages[i].stage);
    }

    //add the backend API
    initializeAPI();
}

RenderPipeline::~RenderPipeline() noexcept
{
    //only delete if the API is set up
    if (m_api)
    {
        //switch over the API to delete the correct type
        switch (GLGE::Graphic::Backend::INSTANCE.getAPI())
        {
        case GLGE_GRAPHIC_INSTANCE_API_OPEN_GL:
            delete (GLGE::Graphic::Backend::OGL::RenderPipeline*)m_api;
            break;
        
        default:
            break;
        }
        //null out the API
        m_api = nullptr;
    }
}

void RenderPipeline::initializeAPI() noexcept
{
    //switch over the used API
    switch (GLGE::Graphic::Backend::INSTANCE.getAPI())
    {
    //use OpenGL
    case GLGE_GRAPHIC_INSTANCE_API_OPEN_GL:
        m_api = new GLGE::Graphic::Backend::OGL::RenderPipeline(this);
        break;
    
    default:
        break;
    }
}