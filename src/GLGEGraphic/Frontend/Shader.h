/**
 * @file Shader.h
 * @author DM8AT
 * @brief define what a shader in the frontend is
 * @version 0.1
 * @date 2025-11-01
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_GRAPHIC_FRONTEND_SHADER_
#define _GLGE_GRAPHIC_FRONTEND_SHADER_

//add bools for C
#include <stdbool.h>
//add strings to work with string data
#include "../../GLGE_BG/CBinding/String.h"

/**
 * @brief define the different kinds of shader stages
 */
typedef enum e_ShaderStageType {
    //the stage contains data for a fragment shader stage
    SHADER_STAGE_FRAGMENT = 0,
    //the stage contains data for a vertex shader stage
    SHADER_STAGE_VERTEX,
    //the stage contains data for a geometry shader stage
    SHADER_STAGE_GEOMETRY,
    //the stage contains data for a tesselation control shader stage
    SHADER_STAGE_TESSELATION_CONTROL,
    //the stage contains data for a tesselation evaluation shader stage
    SHADER_STAGE_TESSELATION_EVALUATION,
    //the stage contains data for a compute shader
    SHADER_STAGE_COMPUTE
} ShaderStageType;

//define how many different shader stages exist
#define GLGE_SHADER_STAGE_TYPE_COUNT 6

/**
 * @brief define the type of shader source code given
 */
typedef enum s_ShaderSourceType {
    //the shader source code is provided as GLSL source code
    SHADER_SOURCE_GLSL = 0,
    //the shader source code is provided as SPIR-V bytecode
    SHADER_SOURCE_SPIR_V,
} ShaderSourceType;

/**
 * @brief define what the structure of a single shader stage is
 */
typedef struct s_ShaderStage {
    //store the source code for this shader stage
    String sourceCode;
    //store the type of the stored source code
    ShaderSourceType srcType;
    //store the type of stage this is
    ShaderStageType stage;
} ShaderStage;

//define classes only for C++
#if __cplusplus

//add arrays for structured data storage
#include <array>

/**
 * @brief define what a frontend shader is
 */
class Shader
{
public:

    /**
     * @brief Construct a new Shader
     * 
     * @warning each stage may exist ONCE
     * 
     * @param stages a list of all the shader stages to create the shader from
     */
    Shader(std::initializer_list<ShaderStage> stages) noexcept
     : Shader(stages.begin(), stages.end() - stages.begin())
    {}

    /**
     * @brief Construct a new Shader
     * 
     * @warning each stage may exist ONCE
     * 
     * @param stages a C array of stages to create the shader from
     * @param stageCount the amount of shaders in the array
     */
    Shader(const ShaderStage* stages, uint64_t stageCount) noexcept;

    /**
     * @brief Destroy the Shader
     */
    ~Shader();

    /**
     * @brief Get the Shader Bitmask that defines what type of shader are used
     * 
     * @return uint8_t the bitmask containing the information
     */
    inline uint8_t getShaderBitmask() const noexcept {return m_used;}

    /**
     * @brief Get the Shader Stages of the shader
     * 
     * @return const ShaderStage* a pointer to the array of shader stage
     */
    inline const ShaderStage* getShaderStages() const noexcept {return m_stages.data();}

protected:

    //store all shader stages just in-place (good for RAM layout)
    //also, this makes it possible to just use the enum value as index to the array
    //so this works as a very cheap "hash" map (the key is known -> it is just the index)
    std::array<ShaderStage, GLGE_SHADER_STAGE_TYPE_COUNT> m_stages;
    //store a bit mask of all used shader stages
    uint8_t m_used = 0;
    //store the backend implementation for the shader
    void* m_shader = nullptr;

};

#endif

#if __cplusplus
extern "C" {
#endif

/**
 * @brief check if the shader compile function should be called
 * 
 * @return true : the shader compiler should be called
 * @return false : no need to call the shader compiler, no compilations are queued
 */
bool glge_Shader_RequiresCompile();

/**
 * @brief compile all queued up shader
 */
void glge_Shader_Compile();

#if __cplusplus
}
#endif

#endif