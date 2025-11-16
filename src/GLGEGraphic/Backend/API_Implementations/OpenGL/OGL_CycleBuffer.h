/**
 * @file OGL_CycleBuffer.h
 * @author DM8AT
 * @brief define the API overload for a backend buffer of a cycle buffer for OpenGL
 * @version 0.1
 * @date 2025-11-15
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//header guard
#ifndef _GLGE_GRAPHIC_BACKEND_API_IMPL_OGL_OGL_CYCLE_BUFFER_
#define _GLGE_GRAPHIC_BACKEND_API_IMPL_OGL_OGL_CYCLE_BUFFER_

//add the cycle buffer API
#include "../API_CycleBuffer.h"

//only available for C++
#if __cplusplus

//add a vector that is used to queue up updates for the cycle buffer backend
#include <vector>
//add a mutex to make the cycle buffer backend thread safe
#include <mutex>
#include <shared_mutex>

//use the namespace GLGE::Graphic::Backend::OGL
namespace GLGE::Graphic::Backend::OGL
{

/**
 * @brief the pure virtual base class for each GPU buffer of a cycle buffer
 */
class CycleBufferBackend : public API::CycleBufferBackend
{
public:
    /**
     * @brief Construct a new Cycle Buffer Backend
     * 
     * @param cycleBuffer a pointer to the cycle buffer this backend belongs to
     * @param idx the index of the buffer backend
     */
    CycleBufferBackend(API::CycleBuffer* cycleBuffer, uint8_t idx);

    /**
     * @brief Destroy the Cycle Buffer Backend
     */
    virtual ~CycleBufferBackend() {m_cBuff = nullptr;}

    /**
     * @brief sync the GPU and CPU data with the data from the cycle buffer
     * @warning this should only be called from the main thread during the main graphic update
     */
    virtual void syncCPU() noexcept override;

    /**
     * @brief Get the OpenGL Buffer
     * 
     * @return uint32_t the OpenGL buffer name
     */
    inline uint32_t getBuffer() const noexcept {return m_buff;}

    /**
     * @brief mark that this buffer is now the GPU consumed buffer
     * @warning this should only be called from the main thread during the main graphic update
     */
    virtual void markInUse() noexcept override;

protected:

    /**
     * @brief update the OpenGL buffer backend
     */
    void update() noexcept;

    /**
     * @brief queue this cycle buffer backend instance for updating
     */
    void queueUpdate() noexcept;

    /**
     * @brief 
     */
    void syncGPU() noexcept;

    //add instances as a friend class
    friend class Instance;

    //store the OpenGL buffer name
    uint32_t m_buff = 0;
    //store the size of the buffer
    uint64_t m_size = 0;
    //store a synchronization primitive
    void* m_sync = nullptr;
    //store the mapped pointer
    void* m_mapped = nullptr;
    //store if an update is requested
    std::atomic_bool m_update{false};

    //store a mutex to make the update queue thread safe
    inline static std::shared_mutex m_updateMtx;
    //store a queue of all backends that need to be updated from the main thread
    inline static std::vector<OGL::CycleBufferBackend*> m_updateQueue;

};

}

#endif

#endif