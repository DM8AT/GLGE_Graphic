/**
 * @file OGL_Shader.cpp
 * @author DM8AT
 * @brief implement the OpenGL shader
 * @version 0.1
 * @date 2025-11-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//add the OpenGL shader
#include "OGL_Shader.h"

//add OpenGL command buffer
#include "OGL_CommandBuffer.h"

//add OpenGL
#include "glad/glad.h"
//for error printing
#include <iostream>

//add the frontend shader
#include "../../../Frontend/Shader.h"

/**
 * @brief Get the amount of true-bits (1 bits) from a bitmask
 * 
 * @param bitmask the bitmask to check
 * @return uint8_t the amount of set bits
 */
static uint8_t getBitTrueCount(uint8_t bitmask) noexcept
{
    //store the amount
    uint8_t count = 0;
    //loop while there are still bits in the mask
    while (bitmask) {
        //remove the most significant bit
        bitmask &= (bitmask - 1);
        ++count;
    }
    //return the final count
    return count;
}

/**
 * @brief Get the Most Significant Bit of a bit mask
 * 
 * @param bitmask the bitmask to quarry the most significant bit from
 * @return uint8_t the index of the most significant bit
 */
static uint8_t getMostSignificantBit(uint8_t bitmask) noexcept
{
    //sanity check the bitmask
    if (bitmask == 0) return UINT8_MAX;

    //store the position of the bit
    uint8_t pos = 0;

    //just use an if - else if snake (8 positions possible, nothing fancy required)
    if (bitmask & 0x80)      {pos = 7;}
    else if (bitmask & 0x40) {pos = 6;}
    else if (bitmask & 0x20) {pos = 5;}
    else if (bitmask & 0x10) {pos = 4;}
    else if (bitmask & 0x08) {pos = 3;}
    else if (bitmask & 0x04) {pos = 2;}
    else if (bitmask & 0x02) {pos = 1;}
    else if (bitmask & 0x01) {pos = 0;}

    //return the final position
    return pos;
}

/**
 * @brief convert a GLGE shader stage type to a GLenum shader stage
 * 
 * @param stageType the shader type enum to convert
 * @return GLenum the corresponding OpenGL shader stage
 */
static GLenum __glgeShaderToGLShader(ShaderStageType stageType) {
    switch (stageType)
    {
    case SHADER_STAGE_FRAGMENT:
        return GL_FRAGMENT_SHADER;
        break;
    case SHADER_STAGE_VERTEX:
        return GL_VERTEX_SHADER;
        break;
    case SHADER_STAGE_GEOMETRY:
        return GL_GEOMETRY_SHADER;
        break;
    case SHADER_STAGE_TESSELATION_CONTROL:
        return GL_TESS_CONTROL_SHADER;
        break;
    case SHADER_STAGE_TESSELATION_EVALUATION:
        return GL_TESS_EVALUATION_SHADER;
        break;
    case SHADER_STAGE_COMPUTE:
        return GL_COMPUTE_SHADER;
        break;
    
    default:
        return 0;
        break;
    }
}

/**
 * @brief convert an OpenGL shader type enum to a string
 * 
 * @param shaderType the shader type to quarry the string for
 * @return const char* the string for the shader type
 */
static const char* __glShaderTypeToString(GLenum shaderType) noexcept
{
    switch (shaderType)
    {
    case GL_VERTEX_SHADER:
        return "Vertex Shader";
        break;
    case GL_FRAGMENT_SHADER:
        return "Fragment Shader";
        break;
    case GL_GEOMETRY_SHADER:
        return "Geometry Shader";
        break;
    case GL_TESS_CONTROL_SHADER:
        return "Tesselation Control Shader";
        break;
    case GL_TESS_EVALUATION_SHADER:
        return "Tesselation Evaluation Shader";
        break;
    case GL_COMPUTE_SHADER:
        return "Compute Shader";
        break;
    
    default:
        return "Invalid Shader Type";
        break;
    }
}

/**
 * @brief Create a Shader object
 * 
 * @param src the source code for the shader
 * @param srcLen the amount of characters in the source code
 * @param shaderType the type of shader unit to create
 * @return GLuint the new shader object or 0 on error
 */
static GLuint createShaderObject(const char* src, uint64_t srcLen, GLenum shaderType) noexcept
{
    //create the shader object
    GLuint shader = glCreateShader(shaderType);
    if (shader == 0) {
        std::cerr << "Error: Failed to create shader of type " << __glShaderTypeToString(shaderType) << std::endl;
        return 0;
    }

    //set the shader source code
    glShaderSource(shader, 1, &src, (GLint*)&srcLen);
    
    //compile the shader
    glCompileShader(shader);
    
    //check for compilation errors
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        //retrieve and print the error log
        GLint logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        //only print the log if a log exists
        if (logLength > 1) {
            std::string log(logLength, '\0');
            glGetShaderInfoLog(shader, logLength, &logLength, &log[0]);
            std::cerr << "Error: Shader compilation failed.\n" << log << std::endl;
        }
        
        //clean up and return 0
        glDeleteShader(shader); 
        return 0;
    }

    //return the successfully compiled shader
    return shader;
}

void GLGE::Graphic::Backend::OGL::Shader::compile() noexcept
{
    //update the state
    m_state.store(CompileState::COMPILE_ONGOING, std::memory_order_release);

    //create the actual program
    m_prog = glCreateProgram();

    //prepare a vector to store all the shader in
    std::vector<GLuint> shader;
    shader.reserve(getBitTrueCount(m_shader->getShaderBitmask()));
    //store the bitmask of still to do shader
    uint8_t todo = m_shader->getShaderBitmask();
    //iterate over all existing shader
    while (todo) {
        //pop the top most shader stage off
        uint8_t idx = getMostSignificantBit(todo);
        todo &= ~(1 << idx);
        //compile the shader stage
        GLuint stage = createShaderObject(m_shader->getShaderStages()[idx].sourceCode.c_str(), 
                                          m_shader->getShaderStages()[idx].sourceCode.size(), 
                                          __glgeShaderToGLShader(m_shader->getShaderStages()[idx].stage));
        //sanity check
        if (stage == 0) {
            //clean up
            for (size_t i = 0; i < shader.size(); ++i) {
                glDeleteShader(shader[i]);
            }
            glDeleteProgram(m_prog);
            m_prog = 0;

            m_state.store(CompileState::COMPILE_FAILED, std::memory_order_release);
            return;
        }
        //on success store the shader
        shader.push_back(stage);
        //link the shader
        glAttachShader(m_prog, stage);
    }

    //link the program
    glLinkProgram(m_prog);

    //check if the program linking was successful
    GLint success = 0;
    glGetProgramiv(m_prog, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        //retrieve and print the error log
        GLint logLength = 0;
        glGetProgramiv(m_prog, GL_INFO_LOG_LENGTH, &logLength);

        //only print an error if an error exiss
        if (logLength > 1) {
            std::string log(logLength, '\0');
            glGetProgramInfoLog(m_prog, logLength, &logLength, &log[0]);
            std::cerr << "Error: Shader program linking failed.\n" << log << std::endl;
        }

        //clean up the shaders and program
        for (size_t i = 0; i < shader.size(); ++i) {
            glDeleteShader(shader[i]);
        }
        glDeleteProgram(m_prog);
        m_prog = 0;

        //update the state to failed linking
        m_state.store(CompileState::COMPILE_FAILED, std::memory_order_release);
        return;
    }

    //successfully linked, so finalize
    //detach shaders after linking
    for (size_t i = 0; i < shader.size(); ++i) {
        glDetachShader(m_prog, shader[i]);
        glDeleteShader(shader[i]);
    }

    //update the program state to indicate successful compilation
    m_state.store(CompileState::COMPILE_SUCCESS, std::memory_order_release);
}