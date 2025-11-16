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
#include "../../../Frontend/RenderAPI/RenderMesh.h"

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
     * @param stageIndex the id of the stage
     */
    virtual void execute(const RenderPipelineStage& stage, uint64_t stageIndex) noexcept;

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
    void executeStage_Custom(const RenderPipelineStageData& stage) noexcept;

    /**
     * @brief draw a simple render mesh
     * 
     * @param stage the stage data to execute on
     */
    void executeStage_SimpleDrawRenderMesh(const RenderPipelineStageData& stage) noexcept;

    /**
     * @brief draw a whole scene
     * 
     * @param stage the stage data to execute on
     */
    void executeStage_DrawScene(const RenderPipelineStageData& stage) noexcept;

    /**
     * @brief dispatch a compute shader
     * 
     * @param stage the stage data to execute on
     */
    void executeStage_DispatchCompute(const RenderPipelineStageData& stage) noexcept;

    /**
     * @brief run a memory barrier
     * 
     * @param stage the stage data to execute on
     */
    void executeStage_MemoryBarrier(const RenderPipelineStageData& stage) noexcept;

    /**
     * @brief clean up the old data
     */
    void executeStage_Cleanup(const RenderPipelineStageData&) noexcept;

    /**
     * @brief finalize the recording
     */
    void executeStage_Finalize(const RenderPipelineStageData&) noexcept;

    /**
     * @brief store the OpenGL command buffer
     */
    CommandBuffer m_cmdBuff;
    /**
     * @brief store the clear color of the parent window
     */
    vec4 m_clearColor;

    //store a list of all command-made buffers
    std::vector<uint32_t> m_customBuffs;
    //store a list of function pointers that are delayed till the first run
    //this stores stages to execute later
    std::vector<std::pair<RenderPipelineStageData, void (RenderPipeline::*)(const RenderPipelineStageData& stage)>> m_todo;

};

};

#endif

#endif