/**
 * @file RenderPipeline.h
 * @author DM8AT
 * @brief define the structure for a render pipeline
 * 
 * a render pipeline is a structure that is used to define what, when and with what is used to render to something
 * 
 * @version 0.1
 * @date 2025-10-17
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_GRAPHICS_FRONTEND_RENDER_PIPELINE_
#define _GLGE_GRAPHICS_FRONTEND_RENDER_PIPELINE_

//add string types
#include "../../GLGE_BG/CBinding/String.h"
//add windows to the render pipeline (pipelines operate on windows)
#include "Window/Window.h"

/**
 * @brief define all types of stages a render pipeline may include
 */
typedef enum e_RenderPipelineStageType {
    /**
     * @brief a stage that calls a custom function from the render thread
     * 
     * Do not write your whole program using this stage, it will be slow
     */
    GLGE_RENDER_PIPELINE_STAGE_CUSTOM = 0
} RenderPipelineStageType;

/**
 * @brief define what the data for a render pipeline stage may look like
 */
typedef union u_RenderPipelineStageData {
    /**
     * @brief store the data for a custom stage
     */
    struct CustomStage {
        /**
         * @brief store the function to call. It should return nothing and take in the user data (even if it is unused)
         */
        void (*custom_func)(void*);
        /**
         * @brief store some user-define data that is passed to the function
         */
        void* userData;
    } customStage;
} RenderPipelineStageData;

/**
 * @brief define what a single stage of the render pipeline contains
 */
typedef struct s_RenderPipelineStage {
    /**
     * @brief store the type of the stage
     */
    RenderPipelineStageType type;
    /**
     * @brief store the data for the stage
     */
    RenderPipelineStageData data;
} RenderPipelineStage;

/**
 * @brief just a pairing of a render pipeline stage and a name
 */
typedef struct s_RenderPipelineStageNamed {
    /**
     * @brief store the name of the stage
     */
    String name;
    /**
     * @brief store the actual pipeline stage
     */
    RenderPipelineStage stage;
} RenderPipelineStageNamed;

//define a class for C++ and an opaque object for C
#if __cplusplus

//add maps for the storage of render pipeline stages
#include <map>
//atomic stuff is required
#include <atomic>
//threading required
#include <thread>
//conditional variable is required for synchronization
#include <condition_variable>

/**
 * @brief define a render pipeline
 */
class RenderPipeline
{
public:

    /**
     * @brief Construct a new Render Pipeline from a list of stages. Order is important and retained. 
     * 
     * @param stages a map of stages to execute in the given order
     * @param window the window to operate on (null = the pipeline operates on no windows, but may still operate on framebuffers or buffers)
     */
    RenderPipeline(const std::map<String, RenderPipelineStage> stages, Window* window);

    /**
     * @brief Construct a new Render Pipeline from a list of named stages. Order is important. 
     * 
     * @param namedStages a C array of named render pipeline stages
     * @param namedStageCount the amount of elements in the list
     * @param window the window to operate on (null = the pipeline operates on no windows, but may still operate on framebuffers or buffers)
     */
    RenderPipeline(const RenderPipelineStageNamed* namedStages, size_t namedStageCount, Window* window);

    /**
     * @brief Destroy the Render Pipeline
     */
    ~RenderPipeline() noexcept;

    /**
     * @brief Get a specific stage from the render pipeline
     * 
     * @warning it is not checked if the name actually corresponds to a stage. If the name does not exist in the render pipeline this function will result in undefined behaviour. 
     * 
     * @param name the name of the stage to quarry
     * @return const RenderPipelineStage& a constant reference to the render pipeline stage to quarry
     */
    inline const RenderPipelineStage& getStage(const String& name) {return m_stages[name];}

    /**
     * @brief check if the render pipeline contains a specific stage
     * 
     * @param name the name of the stage to quarry
     * @return true : the name corresponds to a stage in the render pipeline
     * @return false : the stage does not correspond to a render pipeline
     */
    inline bool containsStage(const String& name) const noexcept {return m_stages.find(name) != m_stages.end();}

    /**
     * @brief Get the Stages of the render pipeline
     * 
     * @return const std::map<String, RenderPipelineStage> a constant reference to the render pipeline
     */
    inline const std::map<String, RenderPipelineStage>& getStages() const noexcept {return m_stages;}

    /**
     * @brief record the whole command buffer
     */
    void record() noexcept;

    /**
     * @brief wait for all recordings to end
     */
    void waitForRecording() noexcept;

    /**
     * @brief play the command buffer
     */
    void play() noexcept;

    /**
     * @brief Get the Window the pipeline operates on
     * 
     * @return Window* a pointer to the window the pipeline operates on or NULL if it does not operate on a window
     */
    inline ::Window* getWindow() const noexcept {return m_window;}

protected:

    /**
     * @brief change the state of the is recording boolean
     * 
     * @param state the new state for the recording
     */
    void updateRecordingState(bool state) noexcept;

    /**
     * @brief wait till the recording state has reached a specific state
     * 
     * @param state the state to wait for
     */
    void waitForRecordingState(bool state) noexcept;

    //initialize the backend API
    void initializeAPI() noexcept;

    //async recording function
    void asyncRecord() noexcept;

    //store a map of the render pipeline stages to the names
    std::map<String, RenderPipelineStage> m_stages;

    //store the API implementation for the render pipeline
    void* m_api = nullptr;
    //store the thread for recording
    std::thread m_thread;
    //sync stuff
    std::mutex m_mut;
    std::condition_variable m_cond;
    //store if the render pipeline is recording
    std::atomic_bool m_isRecording = false;
    //store the window the render pipeline operates on
    ::Window* m_window = nullptr;

};

#endif

#endif