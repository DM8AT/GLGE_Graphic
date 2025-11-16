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
//add frontend materials
#include "../../../Frontend/Material.h"
//add scenes
#include "../../../../GLGE_Core/Geometry/Structure/ECS/Scene.h"
//add renderers to access render-related data
#include "../../../Frontend/RenderAPI/Renderer.h"

//unordered maps are used to store the mapping from material -> list of meshes
#include <unordered_map>

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

    case GLGE_RENDER_PIPELINE_STAGE_SIMPLE_DRAW_RENDER_MESH:
        executeStage_SimpleDrawRenderMesh(stage.data.simpleDrawRenderMesh);
        break;

    case GLGE_RENDER_PIPELINE_STAGE_DRAW_SCENE:
        executeStage_DrawScene(stage.data.drawScene);
        break;

    case GLGE_RENDER_PIPELINE_DISPATCH_COMPUTE:
        executeStage_DispatchCompute(stage.data.dispatchCompute);
        break;

    case GLGE_RENDER_PIPELINE_MEMORY_BARRIER:
        m_cmdBuff.record<Command_MemoryBarrier>();
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

void GLGE::Graphic::Backend::OGL::RenderPipeline::executeStage_SimpleDrawRenderMesh(const RenderPipelineStageData::SimpleDrawRenderMesh& stage) noexcept
{
    //bind the material of the render mesh
    m_cmdBuff.record<Command_BindMaterial>((OGL::Material*)((::Material*)stage.material)->getBackend());
    //draw the render mesh
    m_cmdBuff.record<Command_DrawMesh>((API::RenderMesh*)(RenderMeshRegistry::get(stage.handle))->getBackend());
}

void GLGE::Graphic::Backend::OGL::RenderPipeline::executeStage_DrawScene(const RenderPipelineStageData::DrawScene& stage) noexcept
{
    //BATCHING STEP

    //store all the batches
    //the batches are stored by mapping a material pointer to an std::vector of render mesh handles
    std::unordered_map<::Material*, std::vector<RenderMeshHandle>> batches;

    //get all objects from the scene that have a renderer component
    std::vector<std::pair<Object, Renderer*>> toBatch = ((Scene*)stage.scene)->get<Renderer>();
    //iterate over all object - renderer pairs
    for (auto& pair : toBatch) {
        //iterate over all mesh - material pairs in the renderer
        for (size_t i = 0; i < pair.second->getElementCount(); ++i) {
            //check if the material is known
            const RenderObject& obj = pair.second->getObject(i);
            auto pos = batches.find(obj.material);
            if (pos == batches.end()) {
                //if not, create a new entry
                batches.emplace(obj.material, std::initializer_list{obj.handle});
            } else {
                //if it exists, just add the handle to the map
                pos->second.push_back(obj.handle);
            }
        }
    }

    //DRAWING STEP

    //iterate over all computed batches
    for (auto& batch : batches) {
        //bind the material
        m_cmdBuff.record<Command_BindMaterial>((OGL::Material*)batch.first->getBackend());

        //iterate over all meshes in the batch
        for (auto& mesh : batch.second) {
            m_cmdBuff.record<Command_DrawMesh>((API::RenderMesh*)RenderMeshRegistry::get(mesh)->getBackend());
        }
    }
}

void GLGE::Graphic::Backend::OGL::RenderPipeline::executeStage_DispatchCompute(const RenderPipelineStageData::DispatchCompute& stage) noexcept
{
    //simply queue the dispatch compute command
    m_cmdBuff.record<Command_DispatchCompute>(stage.compute, stage.instances[0], stage.instances[1], stage.instances[2]);
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