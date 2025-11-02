/**
 * @file OGL_Shader.h
 * @author DM8AT
 * @brief define the shader for the OpenGL backend
 * @version 0.1
 * @date 2025-11-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//header guard
#ifndef _GLGE_GRAPHIC_BACKEND_API_IMPL_OGL_SHADER_
#define _GLGE_GRAPHIC_BACKEND_API_IMPL_OGL_SHADER_

//only available for C++
#if __cplusplus

//add the shader API
#include "../API_Shader.h"

//use the namespace GLGE::Graphic::Backend::OGL
namespace GLGE::Graphic::Backend::OGL
{

/**
 * @brief define the shader backend class for OpenGL
 */
class Shader final : public API::Shader
{
public:

    /**
     * @brief Construct a new Shader object
     * 
     * @param shader 
     */
    Shader(::Shader* shader) : API::Shader(shader) {
        //add this shader to the compile queue
        std::shared_lock lock(m_compileMutex);
        m_compileQueue.push_back(this);
    }

protected:

    /**
     * @brief compile the OpenGL shader
     */
    virtual void compile() noexcept override;

    //store the compiled OpenGL shader program
    uint32_t m_prog = 0;

};

}

#endif

#endif