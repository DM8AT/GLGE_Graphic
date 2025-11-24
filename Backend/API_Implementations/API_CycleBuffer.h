/**
 * @file API_CycleBuffer.h
 * @author DM8AT
 * @brief define a buffer that is specialized on quick uploads
 * @version 0.1
 * @date 2025-11-15
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//header guard
#ifndef _GLGE_GRAPHIC_BACKEND_API_IMPL_API_CYCLE_BUFFER_
#define _GLGE_GRAPHIC_BACKEND_API_IMPL_API_CYCLE_BUFFER_

//add types
#include "../../../../GLGE_Core/Types.h"
//add the buffer API
#include "API_Buffer.h"

//store the maximum amount of buffers a single buffer class may use for ring buffering
#define GLGE_BUFFER_MAX_RING_BUFFER_COUNT 4

//define the cycle buffer class only for C++
#if __cplusplus

//add atomics for thread safety
#include <atomic>
//add vectors for queues
#include <vector>
//add a mutex for thread safety
#include <shared_mutex>

//use the GLGE::Graphic::Backend::API namespace
namespace GLGE::Graphic::Backend::API
{

//cycle buffers will be defined later
class CycleBuffer;

/**
 * @brief the pure virtual base class for each GPU buffer of a cycle buffer
 */
class CycleBufferBackend
{
public:

    /**
     * @brief Construct a new Cycle Buffer Backend
     * 
     * @param cycleBuffer a pointer to the cycle buffer this backend belongs to
     * @param idx the index of the buffer backend
     */
    CycleBufferBackend(CycleBuffer* cycleBuffer, uint8_t idx)
     : m_cBuff(cycleBuffer), m_idx(idx)
    {}

    /**
     * @brief Destroy the Cycle Buffer Backend
     */
    virtual ~CycleBufferBackend() {m_cBuff = nullptr;}

    /**
     * @brief sync the GPU and CPU data with the data from the cycle buffer
     * @warning this should only be called from the main thread during the main graphic update
     * 
     * @param force true : the buffer syncing is forced regardless of GPU state | false : the buffer MAY sync, but it can choose not to. 
     */
    virtual void syncCPU(bool force) noexcept = 0;

    /**
     * @brief mark that this buffer is now the GPU consumed buffer
     * @warning this should only be called from the main thread during the main graphic update
     */
    virtual void markInUse() noexcept = 0;

protected:

    //store the cycle buffer the buffer belongs to
    CycleBuffer* m_cBuff = nullptr;
    //store the index of the buffer
    uint8_t m_idx = UINT8_MAX;
    //store the version the buffer was updated last
    uint32_t m_version = 0;

};

/**
 * @brief define the API for a cycle buffer
 */
class CycleBuffer
{
public:

    /**
     * @brief Construct a new Cycle Buffer
     * 
     * @param data the raw data to initialize the buffer with
     * @param size the size of the data in bytes
     * @param type the type of buffer(s) to create
     * @param bufferCount the amount of buffers to create. Must be between 1 and GLGE_BUFFER_MAX_RING_BUFFER_COUNT
     */
    CycleBuffer(void* data, uint64_t size, Buffer::Type type, uint8_t bufferCount = 1);

    /**
     * @brief Destroy the Cycle Buffer
     */
    ~CycleBuffer();

    /**
     * @brief tick the cycle buffer
     * @warning DO NOT USE! this happens automatically by a window before the rendering starts. 
     */
    void tick() noexcept;

    /**
     * @brief Get the buffer that is currently ready for GPU consumption
     * 
     * @tparam T the type of buffer the backend expects
     * @return CycleBufferBackend* a pointer to the current GPU buffer backend
     */
    template <typename T>
    inline T* getCurrentGPUBackend() const noexcept
    {return (T*)m_backends[m_gpuBuff.load(std::memory_order_acquire)];}

    /**
     * @brief Get the Size of the buffer
     * 
     * @return uint64_t the size of the buffer in bytes
     */
    inline uint64_t getSize() const noexcept {return m_size;}

    /**
     * @brief Get the Raw data of the CPU side buffer
     * 
     * @return void* a pointer to the raw CPU side data
     */
    inline void* getRaw() const noexcept {return m_data;}

    /**
     * @brief set the data of the buffer
     * 
     * @param data the new raw data for the buffer
     * @param size the size of the new data in bytes
     */
    void set(void* data, uint64_t size) noexcept;

    /**
     * @brief write the data for a section of the buffer
     * 
     * @param data the data to write to the section
     * @param size the size of the data to write
     * @param offset the byte offset from the start of the buffer
     */
    void write(void* data, uint64_t size, uint64_t offset) noexcept;

    /**
     * @brief change the size of the buffer
     * @warning this may truncate data
     * 
     * @param size the new size of the buffer
     */
    void resize(uint64_t size) noexcept;

    /**
     * @brief get the index of the current GPU buffer
     * 
     * @return uint8_t the current GPU buffer
     */
    inline uint8_t getGPUIdx() const noexcept {return m_gpuBuff.load(std::memory_order_acquire);}

    /**
     * @brief Get the Version of the cycle buffer
     * 
     * @return uint32_t the current version
     */
    inline uint32_t getVersion() const noexcept {return m_version.load(std::memory_order_acquire);}

    //store all the cycle buffers
    inline static std::vector<API::CycleBuffer*> s_buffers;
    //store a mutex to make the buffer vector thread safe
    inline static std::shared_mutex s_mtx;

protected:

    //store the size of the buffer
    uint64_t m_size = 0;
    //store the raw data
    void* m_data = nullptr;
    //store the buffer type
    Buffer::Type m_type;
    //store the amount of cycle buffer backends that are in use
    const uint8_t cm_usedBuffers = 0;
    //store the current GPU buffer
    std::atomic_uint8_t m_gpuBuff{0};
    //store the current version
    std::atomic_uint32_t m_version{0};
    //make the buffer thread safe
    std::mutex m_mtx;
    //store all the cycle buffer backends
    CycleBufferBackend* m_backends[GLGE_BUFFER_MAX_RING_BUFFER_COUNT] = { nullptr };

};

}

#endif

#endif