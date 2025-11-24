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
#include "../../../GLGE_BG/Debugging/Logging/__BG_SimpleDebug.h"

//add instances
#include "../../Backend/Instance.h"
//add all API implementations
#include "../../Backend/API_Implementations/AllImplementations.h"

RenderPipeline::RenderPipeline(const std::vector<std::pair<String, RenderPipelineStage>> stages, Window* window)
 : m_window(window)
{
    //make enough spaces
    m_stages.reserve(stages.size());
    m_keyMap.reserve(stages.size());
    //convert the inputted vector to the internal format
    for (const auto& [key, value] : stages) {
        //sanity check
        GLGE_DEBUG_ASSERT("Adding a stage to an allready existing name - overriding the original stage", containsStage(key));
        //store the index of the element
        m_keyMap.insert_or_assign(key, m_stages.size());
        //store the actual element
        m_stages.push_back(value);
    }

    //add the backend API
    initializeAPI();
}

RenderPipeline::RenderPipeline(const RenderPipelineStageNamed* namedStages, size_t namedStageCount, Window* window)
 : m_window(window)
{
    //make enough spaces
    m_stages.reserve(namedStageCount);
    m_keyMap.reserve(namedStageCount);
    //convert the inputted vector to the internal format
    for (size_t i = 0; i < namedStageCount; ++i) {
        //extract the key - value pair
        const String& key = namedStages->name;
        const RenderPipelineStage& value = namedStages->stage;
        //sanity check
        GLGE_DEBUG_ASSERT("Adding a stage to an allready existing name - overriding the original stage", containsStage(key));
        //store the index of the element
        m_keyMap.insert_or_assign(key, m_stages.size());
        //store the actual element
        m_stages.push_back(value);
    }

    //add the backend API
    initializeAPI();
}

RenderPipeline::~RenderPipeline() noexcept
{
    //make sure to join back any lingering threads
    if (m_thread.joinable()) {m_thread.join();}
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

void RenderPipeline::record() noexcept
{
    //if any recording is lingering, re-join it
    if (m_thread.joinable()) {m_thread.join();}
    //lock the recording
    updateRecordingState(true);
    //start the async recording
    m_thread = std::thread(&RenderPipeline::asyncRecord, this);
}

void RenderPipeline::waitForRecording() noexcept {
    //wait for the recording state
    waitForRecordingState(false);
    //if the thread can be joined, join it
    if (m_thread.joinable()) {m_thread.join();}
}

void RenderPipeline::play() noexcept
{
    //if delta is 0, the m_last value can't be trusted
    if (m_delta == 0)  {
        m_last = std::chrono::steady_clock::now();
    }

    //store the tick start time
    auto start = std::chrono::steady_clock::now();

    //make sure the the recording is not running
    waitForRecordingState(false);
    //just play back the API pipeline
    ((GLGE::Graphic::Backend::API::RenderPipeline*)m_api)->play();

    //compute the requested iteration rate
    std::chrono::nanoseconds iterRate((int64_t)(1E9 / (double)m_ips));
    //sync up the iteration time
    auto now = std::chrono::steady_clock::now();
    //get the delta
    auto delta = now - m_last;
    m_delta = std::chrono::duration<double>(delta).count();
    //store the new start time
    m_last = now;
    //only wait if requested
    if (m_ips != GLGE_UNLIMITED) {
        //compute the time to wait
        auto toWait = iterRate - (start - now);
        //if the time is valid, wait for the time to pass
        if (toWait.count() > 0) {
            std::this_thread::sleep_for(toWait);
        }
    }
}

void RenderPipeline::updateRecordingState(bool state) noexcept
{
    //lock the mutex
    std::lock_guard<std::mutex> lock(m_mut);
    //and set the recording to true
    m_isRecording = state;
    //notify the change
    m_cond.notify_one();
}

void RenderPipeline::waitForRecordingState(bool state) noexcept
{
    //lock the mutex to sync the conditional variable
    std::unique_lock<std::mutex> lock(m_mut);

    //only waite if the state is mismatched
    if (m_isRecording != state)
    {
        //wait for the conditional variable to be in the correct state
        m_cond.wait(lock, [this, state]{ return (m_isRecording.load() == state);});
    }
}

void RenderPipeline::asyncRecord() noexcept
{
    //just record the API pipeline
    ((GLGE::Graphic::Backend::API::RenderPipeline*)m_api)->record();
    //done
    updateRecordingState(false);
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