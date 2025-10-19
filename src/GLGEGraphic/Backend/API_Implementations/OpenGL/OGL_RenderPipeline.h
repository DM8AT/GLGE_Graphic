/**
 * @file OGL_RenderPipeline.h
 * @author DM8AT
 * @brief define the render pipeline implementation for OpenGL
 * @version 0.1
 * @date 2025-10-19
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_GRAPHIC_BACKEND_API_IMPL_OGL_RENDER_PIPELINE_
#define _GLGE_GRAPHIC_BACKEND_API_IMPL_OGL_RENDER_PIPELINE_

//add the render pipeline API
#include "../API_RenderPipeline.h"

//only available for C++
#if __cplusplus

//add command buffers. They are required by the render pipeline
#include "OGL_CommandBuffer.h"

//use the namespace GLGE::Graphic::Backend::OGL
namespace GLGE::Graphic::Backend::OGL
{

/**
 * @brief define the OpenGL implementation of a render pipeline
 */
class RenderPipeline final : public GLGE::Graphic::Backend::API::RenderPipeline
{
public:

    /**
     * @brief Construct a new Render Pipeline
     * 
     * @param pipe the frontend render pipeline
     */
    RenderPipeline(::RenderPipeline* pipe) : API::RenderPipeline(pipe) {}

    /**
     * @brief Destroy the Render Pipeline
     */
    virtual ~RenderPipeline() {}

    /**
     * @brief execute a single render pipeline stage using the API
     * 
     * @param stage the stage to execute
     */
    virtual void execute(const RenderPipelineStage& stage) noexcept;

    /**
     * @brief record the whole pipeline
     */
    virtual void record() noexcept override;

    /**
     * @brief play back a recorded render pipeline
     */
    inline virtual void play() noexcept override {m_cmdBuff.play();}

protected:

    /**
     * @brief execute a custom stage
     * 
     * @param stage the data of the custom stage to execute
     */
    void executeStage_Custom(const RenderPipelineStageData::CustomStage& stage) noexcept;

    /**
     * @brief run a stage that starts a new frame for a window
     * 
     * @param stage the data for the stage
     */
    void executeStage_WindowFrameStart(const RenderPipelineStageData::WindowFrameStart& stage) noexcept;

    /**
     * @brief run a stage that ends a frame of a window
     * 
     * @param stage the data for the stage
     */
    void executeStage_WindowFrameEnd(const RenderPipelineStageData::WindowFrameEnd& stage) noexcept;

    /**
     * @brief store the OpenGL command buffer
     */
    CommandBuffer m_cmdBuff;

};

};

#endif

#endif