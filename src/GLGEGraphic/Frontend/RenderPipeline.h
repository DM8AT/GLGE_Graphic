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

/**
 * @brief define all types of stages a render pipeline may include
 */
typedef enum e_RenderPipelineStageType {
    /**
     * @brief a stage that calls a custom function from the render thread
     * 
     * Do not write your whole program using this stage, it will be slow
     */
    GLGE_RENDER_PIPELINE_STAGE_CUSTOM = 0,
    /**
     * @brief start a frame for a specific window
     */
    GLGE_RENDER_PIPELINE_STAGE_START_WINDOW_FRAME,
    /**
     * @brief end the frame for a specific window
     */
    GLGE_RENDER_PIPELINE_STAGE_END_WINDOW_FRAME,
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
    /**
     * @brief store the data for the stage that starts the frame for a window
     */
    struct WindowFrameStart {
        /**
         * @brief store a pointer to the window to start
         */
        void* window;
    } windowFrameStart;
    /**
     * @brief store the data used to end a frame
     */
    struct WindowFrameEnd {
        /**
         * @brief the window to end the frame for
         */
        void* window;
    } windowFrameEnd;
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

/**
 * @brief define a render pipeline
 */
class RenderPipeline
{
public:

    /**
     * @brief Construct a new Render Pipeline from a list of stages. Order is important and retained. 
     * 
     * @param stages 
     */
    RenderPipeline(const std::map<String, RenderPipelineStage> stages);

    /**
     * @brief Construct a new Render Pipeline from a list of named stages. Order is important. 
     * 
     * @param namedStages a C array of named render pipeline stages
     * @param namedStageCount the amount of elements in the list
     */
    RenderPipeline(const RenderPipelineStageNamed* namedStages, size_t namedStageCount);

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
    inline const std::map<String, RenderPipelineStage> getStages() const noexcept {return m_stages;}

protected:

    //initialize the backend API
    void initializeAPI() noexcept;

    //store a map of the render pipeline stages to the names
    std::map<String, RenderPipelineStage> m_stages;

    //store the API implementation for the render pipeline
    void* m_api = nullptr;

};

#endif

#endif