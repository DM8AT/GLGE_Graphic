/**
 * @file API_Shader.h
 * @author DM8AT
 * @brief define what the unified API for a shader is
 * @version 0.1
 * @date 2025-11-01
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_GRAPHIC_BACKEND_API_IMPL_SHADER_
#define _GLGE_GRAPHIC_BACKEND_API_IMPL_SHADER_

//only available for C++
#if __cplusplus

//add the opaque frontend shader class
class Shader;

//add vectors for queuing
#include <vector>
//add a mutex and atomics for syncing
#include <mutex>
#include <shared_mutex>
#include <atomic>

//use the GLGE::Graphic::Backend::API namespace
namespace GLGE::Graphic::Backend::API
{

/**
 * @brief define the unified shader API class
 */
class Shader
{
public:

    /**
     * @brief define the state of the compiling of the shader
     */
    enum class CompileState : uint8_t {
        //the compiling is nether queued nor ongoing nor done
        COMPILE_UNDEFINED = 0,
        //the compiling is currently queued
        COMPILE_QUEUED,
        //the compiling is currently ongoing
        COMPILE_ONGOING,
        //the compilation was successful
        COMPILE_SUCCESS,
        //errors occurred during compilation
        COMPILE_FAILED
    };

    /**
     * @brief Construct a new Shader
     * 
     * @param shader a pointer to the frontend shader object the shader belongs to
     */
    Shader(::Shader* shader) : m_shader(shader) {}

    /**
     * @brief Destroy the Shader
     */
    virtual ~Shader() {}

    /**
     * @brief Get the Compile status of the shader
     * 
     * @return CompileState the compile status of the shader
     */
    inline CompileState getCompileState() const noexcept {return m_state.load(std::memory_order_acquire);}

    /**
     * @brief check if there is something to compile
     * 
     * @return true : there are shader to compile
     * @return false : there are no shader needed to be compiled
     */
    inline static bool isCompileRequired() noexcept {return m_compileQueue.size() > 0;}

    /**
     * @brief run the compilation for all queued shader
     * 
     * @warning this should be called from the main thread
     */
    static void compileAll() noexcept;

protected:

    /**
     * @brief a function to compile the actual shader
     */
    virtual void compile() = 0;

    //store a pointer to the frontend shader
    ::Shader* m_shader = nullptr;
    //store the current compile status of the shader
    std::atomic<CompileState> m_state{CompileState::COMPILE_UNDEFINED};

    //store all shader that need to be compiled
    inline static std::vector<Shader*> m_compileQueue;
    //store a mutex to make the compile queue thread safe
    inline static std::shared_mutex m_compileMutex;

};

}

#endif

#endif