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
//add frontend render materials
#include "../../../Frontend/RenderMesh.h"

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
    virtual void play() noexcept override;

    /**
     * @brief Get the Clear Color of the pipeline
     * 
     * @return const vec4& the clear color of the window
     */
    inline const vec4& getClearColor() const noexcept {return m_clearColor;}

protected:

    /**
     * @brief execute a custom stage
     * 
     * @param stage the data of the custom stage to execute
     */
    void executeStage_Custom(const RenderPipelineStageData::CustomStage& stage) noexcept;

    /**
     * @brief draw a simple render mesh
     * 
     * @param stage a pointer to the render mesh to draw
     */
    void executeStage_SimpleDrawRenderMesh(::RenderMesh* stage) noexcept;

    /**
     * @brief store the OpenGL command buffer
     */
    CommandBuffer m_cmdBuff;
    /**
     * @brief store the clear color of the parent window
     */
    vec4 m_clearColor;

};

};

#endif

#endif