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

//add framebuffers
#include "../../../Frontend/Framebuffer.h"
#include "OGL_Framebuffer.h"

//unordered maps are used to store the mapping from material -> list of meshes
#include <unordered_map>

//add OpenGL
#include "glad/glad.h"

//use the namespace
using namespace GLGE::Graphic::Backend::OGL;

/**
 * @brief get the framebuffer target of a specific render target
 * 
 * @param target the target to get the framebuffer from
 * @return uint32_t the OpenGL framebuffer of the target
 */
static uint32_t __GetFramebufferTarget(const RenderTarget& target) {
    //switch depending on the target type
    switch (target.type)
    {
    case GLGE_WINDOW:
        //the window target is always 0 on OpenGL
        return 0;
        break;
    case GLGE_FRAMEBUFFER:
        //return the OpenGL framebuffer
        return ((GLGE::Graphic::Backend::OGL::Framebuffer*)((::Framebuffer*)target.target)->getAPI())->getFBO();
        break;
    
    default:
        //how did we get here?
        GLGE_ABORT("Undefined render target type");
        return UINT32_MAX;
        break;
    }
}

void GLGE::Graphic::Backend::OGL::RenderPipeline::executeStage_Custom(const RenderPipelineStageData& _stage) noexcept
{
    //extract the stage
    const RenderPipelineStageData::CustomStage& stage = _stage.customStage;

    //add the custom stage
    m_cmdBuff.record<Command_Custom>(stage.custom_func, stage.userData);
}

void GLGE::Graphic::Backend::OGL::RenderPipeline::executeStage_SimpleDrawRenderMesh(const RenderPipelineStageData& _stage) noexcept
{
    //extract the stage
    const RenderPipelineStageData::SimpleDrawRenderMesh& stage = _stage.simpleDrawRenderMesh;

    //bind the material of the render mesh
    m_cmdBuff.record<Command_BindMaterial>((OGL::Material*)((::Material*)stage.material)->getBackend());
    //draw the render mesh
    m_cmdBuff.record<Command_DrawMesh>((API::RenderMesh*)(RenderMeshRegistry::get(stage.handle))->getBackend());
}

void GLGE::Graphic::Backend::OGL::RenderPipeline::executeStage_DrawScene(const RenderPipelineStageData& _stage) noexcept
{
    //extract the stage
    const RenderPipelineStageData::DrawScene& stage = _stage.drawScene;

    //BATCHING STEP

    //store all the batches
    //the batches are stored by mapping a material pointer to an std::vector of render mesh handles
    std::map<::Material*, std::vector<std::pair<uint32_t, RenderMeshHandle>>> batches;

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
                batches.emplace(obj.material, std::initializer_list{std::pair<uint32_t, RenderMeshHandle>(
                    pair.second->getRenderObjectHandle(), obj.handle)});
            } else {
                //if it exists, just add the handle to the map
                pos->second.push_back(std::pair<uint32_t, RenderMeshHandle>(pair.second->getRenderObjectHandle(), obj.handle));
            }
        }
    }

    //BATCH PREPARATION STEP

    //prepare all buffers
    std::vector<uint32_t> buffs;
    buffs.resize(batches.size() + 1);
    //create all buffers
    glCreateBuffers(buffs.size(), buffs.data());
    //store the most meshes ever found
    //this is used to set up the draw buffer correctly
    size_t maxMeshCount = 0;
    //iterate over all buffers and fill them with the correct data
    size_t buff = 0;
    for (auto& [material, objList] : batches) {
        //store the object and mesh to draw
        std::vector<uint64_t> pair;
        pair.reserve(objList.size());
        //collect the data
        for (size_t i = 0; i < objList.size(); ++i) 
        {pair.push_back(((uint64_t)objList[i].first) | (((uint64_t)objList[i].second.idx) << 32));}
        //upload the data to the GPU
        glNamedBufferStorage(buffs[buff], pair.size()*sizeof(uint64_t), pair.data(), 0);

        //update the new max mesh count
        maxMeshCount = (maxMeshCount > pair.size()) ? maxMeshCount : pair.size();

        //step to the next buffer
        ++buff;
    }

    //setup the last buffer as a draw buffer
    uint32_t drawBuffer = buffs[buffs.size() - 1];
    //allocate enough data for all draw elements
    //(it is important to know that one indirect draw structure is 20 bytes)
    glNamedBufferStorage(drawBuffer, maxMeshCount*20, nullptr, 0);

    //DRAWING STEP

    //iterate over all computed batches
    //store the current batch id
    size_t batch_id = 0;
    for (auto& batch : batches) {
        //draw the batches
        m_cmdBuff.record<Command_DrawMeshesIndirect>(stage.camera, (OGL::Material*)batch.first->getBackend(), batch.second.size(), 
                                                     buffs[batch_id], drawBuffer, stage.batchShader, stage.batchShaderCount);

        //step the batch id
        ++batch_id;
    }
}

void GLGE::Graphic::Backend::OGL::RenderPipeline::executeStage_DispatchCompute(const RenderPipelineStageData& _stage) noexcept
{
    //extract the stage
    const RenderPipelineStageData::DispatchCompute& stage = _stage.dispatchCompute;

    //simply queue the dispatch compute command
    m_cmdBuff.record<Command_DispatchCompute>(stage.compute, stage.instances[0], stage.instances[1], stage.instances[2]);
}

void GLGE::Graphic::Backend::OGL::RenderPipeline::executeStage_MemoryBarrier(const RenderPipelineStageData&) noexcept
{
    //simply run the memory barrier
    m_cmdBuff.record<Command_MemoryBarrier>();
}

void GLGE::Graphic::Backend::OGL::RenderPipeline::executeStage_Cleanup(const RenderPipelineStageData&) noexcept
{
    //delete all command-made buffer if any exist
    if (m_customBuffs.size()) 
    {glDeleteBuffers(m_customBuffs.size(), m_customBuffs.data());}
    m_customBuffs.clear();
}

void GLGE::Graphic::Backend::OGL::RenderPipeline::executeStage_Finalize(const RenderPipelineStageData&) noexcept
{
    //recording done
    m_cmdBuff.markRecorded();
}

void GLGE::Graphic::Backend::OGL::RenderPipeline::executeStage_Blit(const RenderPipelineStageData& _stage) noexcept
{
    //get the stage
    const RenderPipelineStageData::Blit& stage = _stage.blit;

    //compute the mask of the elements to copy
    uint32_t mask = 0;
    mask |= stage.copyColor ? GL_COLOR_BUFFER_BIT : 0;
    mask |= stage.copyDepth ? GL_DEPTH_BUFFER_BIT : 0;
    mask |= stage.copyStencil ? GL_STENCIL_BUFFER_BIT : 0;

    //extract the framebuffers
    uint32_t from = __GetFramebufferTarget(stage.from.target);
    uint32_t to   = __GetFramebufferTarget(stage.to.target);

    //get the filter
    uint32_t filter = (stage.filter == GLGE_FILTER_MODE_NEAREST) ? GL_NEAREST : GL_LINEAR;

    //record the command
    m_cmdBuff.record<Command_Blit>(from, stage.from.extend, stage.from.offset, to, stage.to.extend, stage.to.offset, filter, mask);
}

void GLGE::Graphic::Backend::OGL::RenderPipeline::executeStage_Clear(const RenderPipelineStageData& _stage) noexcept {
    //get the stage
    const RenderPipelineStageData::Clear& stage = _stage.clear;

    //get the framebuffer
    uint32_t fbuff = ((OGL::Framebuffer*)((::Framebuffer*)stage.fbuff)->getAPI())->getFBO();
    //get the type of the buffer
    uint32_t buffType = GL_COLOR;
    switch (stage.type)
    {
    case GLGE_CLEAR_COLOR:
        break;
    case GLGE_CLEAR_DEPTH:
        buffType = GL_DEPTH;
        break;
    case GLGE_CLEAR_STENCIL:
        buffType = GL_STENCIL;
        break;
    
    default:
        GLGE_ABORT("Invalid type of clear buffer");
        break;
    }
    
    //record the clear command
    m_cmdBuff.record<Command_Clear>(stage.value.r, stage.value.g, stage.value.b, stage.value.a, fbuff, buffType, stage.attachment);
}

void GLGE::Graphic::Backend::OGL::RenderPipeline::record() noexcept
{
    //clean the command buffer
    m_cmdBuff.clear();
    //cash the clear color
    m_clearColor = (m_pipeline->getWindow()) ? m_pipeline->getWindow()->getClearColor() : 0;
    //make enough space in the todo-buffer
    m_todo.resize(m_pipeline->getStages().size() + 2);
    //also add a cleanup stage
    m_todo[0] = std::pair<RenderPipelineStageData, void (RenderPipeline::*)(const RenderPipelineStageData&)>
                (RenderPipelineStageData{}, &RenderPipeline::executeStage_Cleanup);
    //just iterate over all stages and play them
    uint64_t id = 1;
    for (auto it = m_pipeline->getStages().begin(); it != m_pipeline->getStages().end(); ++it)
    {execute(*it, id); ++id;}
    //record the finalization stage
    m_todo[m_todo.size()-1] = std::pair<RenderPipelineStageData, void (RenderPipeline::*)(const RenderPipelineStageData&)>
                              (RenderPipelineStageData{}, &RenderPipeline::executeStage_Finalize);
}

void GLGE::Graphic::Backend::OGL::RenderPipeline::execute(const RenderPipelineStage& stage, uint64_t stageIndex) noexcept
{
    //get the pair of the todo list to write to
    auto& todo = m_todo[stageIndex];
    //store the data
    RenderPipelineStageData data = stage.data;
    //store the function to call
    void (OGL::RenderPipeline::* func)(const RenderPipelineStageData &stage) = nullptr;

    //switch over the stage type to do the correct stuff
    switch (stage.type)
    {
    case GLGE_RENDER_PIPELINE_STAGE_CUSTOM:
        func = &RenderPipeline::executeStage_Custom;
        break;

    case GLGE_RENDER_PIPELINE_STAGE_SIMPLE_DRAW_RENDER_MESH:
        func = &RenderPipeline::executeStage_SimpleDrawRenderMesh;
        break;

    case GLGE_RENDER_PIPELINE_STAGE_DRAW_SCENE:
        func = &RenderPipeline::executeStage_DrawScene;
        break;

    case GLGE_RENDER_PIPELINE_DISPATCH_COMPUTE:
        func = &RenderPipeline::executeStage_DispatchCompute;
        break;

    case GLGE_RENDER_PIPELINE_MEMORY_BARRIER:
        func = &RenderPipeline::executeStage_MemoryBarrier;
        break;

    case GLGE_RENDER_PIPELINE_BLIT:
        func = &RenderPipeline::executeStage_Blit;
        break;

    case GLGE_RENDER_PIPELINE_CLEAR:
        func = &RenderPipeline::executeStage_Clear;
        break;
    
    default:
        GLGE_DEBUG_ABORT("Unknown render pipeline stage");
        break;
    }

    //record the actual stage
    todo.emplace(data, func);
}


void GLGE::Graphic::Backend::OGL::RenderPipeline::play() noexcept
{
    //first, clean up the todo list
    for (size_t i = 0; i < m_todo.size(); ++i) {
        (this->*m_todo[i].value().second)(m_todo[i].value().first);
    }
    //todo list is done
    m_todo.clear();

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