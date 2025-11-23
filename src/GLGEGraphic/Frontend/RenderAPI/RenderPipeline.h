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
#ifndef _GLGE_GRAPHICS_FRONTEND_RENDER_API_RENDER_PIPELINE_
#define _GLGE_GRAPHICS_FRONTEND_RENDER_API_RENDER_PIPELINE_

//add string types
#include "../../../GLGE_BG/CBinding/String.h"
//add windows to the render pipeline (pipelines operate on windows)
#include "../Window/Window.h"
//render mesh registries required for render mesh handles
#include "RenderMeshRegistry.h"
//add render targets
#include "RenderTarget.h"
//add filtering
#include "../FilterMode.h"

//define a structure that holds information about a target for blitting
typedef struct s_BlitTargetInfo {
    //store the render target for blitting
    RenderTarget target;
    //store the extend of the rectangle to blit
    uivec2 extend;
    //store the offset to blit from
    uivec2 offset;

    //for C++ add a default constructor
    #if __cplusplus

    /**
     * @brief Construct a new Blit Target Info 
     * 
     * @param _target the constant reference to the target to copy from / to
     * @param _extend the extend of the rectangle to copy / read from / to
     * @param _offset the offset of the rectangle to copy / read from / to
     */
    s_BlitTargetInfo(const RenderTarget& _target, const uivec2& _extend, const uivec2& _offset)
     : target(_target), extend(_extend), offset(_offset)
    {}

    #endif

} BlitTargetInfo;

//define a value that represents unlimited iterations per second for a render pipeline
#define GLGE_UNLIMITED 0

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
     * @brief a stage that draws a simple render mesh
     * 
     * This should be used for debugging only as it is not really optimized to draw a lot
     */
    GLGE_RENDER_PIPELINE_STAGE_SIMPLE_DRAW_RENDER_MESH,
    /**
     * @brief draw an entire scene
     */
    GLGE_RENDER_PIPELINE_STAGE_DRAW_SCENE,
    /**
     * @brief dispatch a compute shader that needs to run every tick
     */
    GLGE_RENDER_PIPELINE_DISPATCH_COMPUTE,
    /**
     * @brief make sure the memory is fully up to date
     */
    GLGE_RENDER_PIPELINE_MEMORY_BARRIER,
    /**
     * @brief copy the contents from one render target to another
     */
    GLGE_RENDER_PIPELINE_BLIT,
    /**
     * @brief clear a specific attachment of the framebuffer
     */
    GLGE_RENDER_PIPELINE_CLEAR
} RenderPipelineStageType;

//define an enum to map what to clear to colors
typedef enum e_ClearType {
    //clear a color attachment
    GLGE_CLEAR_COLOR = 0,
    //clear the depth attachment
    GLGE_CLEAR_DEPTH = 1,
    //clear the stencil attachment
    GLGE_CLEAR_STENCIL = 2
} ClearType;

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
    //store a reference of the render mesh to draw
    struct SimpleDrawRenderMesh {
        //store the actual mesh to draw
        RenderMeshHandle handle;
        //store the material to use for rendering
        void* material;
    } simpleDrawRenderMesh;
    //store the data that is needed to draw an entire scene
    struct DrawScene {
        //store a pointer to the scene object to draw
        void* scene;
        //store a pointer to the camera to render from
        void* camera;
        //store a list of compute objects to run before drawing each batch
        void** batchShader;
        //store the amount of batch shader
        uint64_t batchShaderCount;
    } drawScene;
    //store the data that is needed to dispatch a compute shader
    struct DispatchCompute {
        //store a pointer to the compute object to dispatch
        void* compute;
        //store the amount of instances to create
        uint32_t instances[3];
    } dispatchCompute;
    //store the data that is needed to copy from one render target to another
    struct Blit {
        //store the target to copy data from
        BlitTargetInfo from;
        //store the target to copy data to
        BlitTargetInfo to;
        //define what filtering to use for blitting
        FilterMode filter;
        //store what to copy
        bool copyColor;
        bool copyDepth;
        bool copyStencil;
    } blit;
    //store the data needed to clear a framebuffer
    struct Clear {
        //a pointer to the framebuffer to clear
        void* fbuff;
        //the color / value to clear with
        vec4 value;
        //store what to clear
        ClearType type;
        //store the attachment to clear (only used for color)
        uint8_t attachment;
    } clear;
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
#include <unordered_map>
//atomic stuff is required
#include <atomic>
//threading required
#include <thread>
//conditional variable is required for synchronization
#include <condition_variable>
//add chrono for timing
#include <chrono>

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
    RenderPipeline(const std::vector<std::pair<String, RenderPipelineStage>> stages, ::Window* window);

    /**
     * @brief Construct a new Render Pipeline from a list of named stages. Order is important. 
     * 
     * @param namedStages a C array of named render pipeline stages
     * @param namedStageCount the amount of elements in the list
     * @param window the window to operate on (null = the pipeline operates on no windows, but may still operate on framebuffers or buffers)
     */
    RenderPipeline(const RenderPipelineStageNamed* namedStages, size_t namedStageCount, ::Window* window);

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
    inline RenderPipelineStage& getStage(const String& name) noexcept 
    {return m_stages[m_keyMap.find(name)->second];}

    /**
     * @brief check if the render pipeline contains a specific stage
     * 
     * @param name the name of the stage to quarry
     * @return true : the name corresponds to a stage in the render pipeline
     * @return false : the stage does not correspond to a render pipeline
     */
    inline bool containsStage(const String& name) const noexcept {return m_keyMap.find(name) != m_keyMap.end();}

    /**
     * @brief Get the Stages of the render pipeline
     * 
     * @return const std::map<String, RenderPipelineStage> a constant reference to the render pipeline
     */
    inline const std::vector<RenderPipelineStage>& getStages() const noexcept {return m_stages;}

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

    /**
     * @brief Set the maximal Iteration Rate of the render pipeline
     * 
     * @param iterRate the maximum repeat rate in iterations per second (use GLGE_UNDEFINED for unlimited iterations per second)
     */
    inline void setIterationRate(uint64_t iterRate) noexcept
    {m_ips = iterRate;}

    /**
     * @brief Get the maximum Iteration Rate of the render pipeline
     * 
     * @return uint64_t the maximum repeat rate in iterations per second (use GLGE_UNDEFINED for unlimited iterations per second)
     */
    inline uint64_t getIterationRate() const noexcept {return m_ips;}

    /**
     * @brief get the delta time of the render pipeline
     * 
     * @return double the delta time in seconds
     */
    inline double getDelta() const noexcept {return m_delta;}

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

    //store the stages in the inputted order
    std::vector<RenderPipelineStage> m_stages;
    //store a map of the render pipeline stages to the names
    std::unordered_map<String, uint32_t> m_keyMap;

    //store the API implementation for the render pipeline
    void* m_api = nullptr;
    //store the requested iterations per second
    uint64_t m_ips = GLGE_UNLIMITED;
    //store the last start
    std::chrono::steady_clock::time_point m_last;
    //store the current delta time
    double m_delta = 0.f;
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