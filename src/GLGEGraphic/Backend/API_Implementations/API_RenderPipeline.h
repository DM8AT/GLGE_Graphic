/**
 * @file API_RenderPipeline.h
 * @author DM8AT
 * @brief define an abstract class for API-specific render pipeline implementations
 * @version 0.1
 * @date 2025-10-18
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_GRAPHIC_BACKEND_API_IMPL_API_RENDER_PIPELINE_
#define _GLGE_GRAPHIC_BACKEND_API_IMPL_API_RENDER_PIPELINE_

//add the frontend render pipeline
#include "../../Frontend/RenderAPI/RenderPipeline.h"

//only available for C++
#if __cplusplus

//use the GLGE::Graphic::Backend::API namespace
namespace GLGE::Graphic::Backend::API
{

/**
 * @brief define a
 */
class RenderPipeline
{
public:

    /**
     * @brief Construct a new Render Pipeline
     */
    RenderPipeline(::RenderPipeline* pipeline)
     : m_pipeline(pipeline)
    {}

    /**
     * @brief Destroy the Render Pipeline
     */
    virtual ~RenderPipeline() {}

    /**
     * @brief execute a single render pipeline stage using the API
     * 
     * @param stage the stage to execute
     * @param stageIndex the id of the stage
     */
    virtual void execute(const RenderPipelineStage& stage, uint64_t stageIndex) noexcept = 0;

    /**
     * @brief record the whole pipeline
     */
    virtual void record() noexcept = 0;

    /**
     * @brief play back a recorded render pipeline
     */
    virtual void play() noexcept = 0;

protected:

    /**
     * @brief store the frontend render pipeline this backend belongs to
     */
    ::RenderPipeline* m_pipeline = nullptr;

};

}

#endif

#endif