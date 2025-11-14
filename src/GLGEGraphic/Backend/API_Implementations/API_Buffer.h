/**
 * @file API_Buffer.h
 * @author DM8AT
 * @brief define the unified API for a general purpose buffer
 * @version 0.1
 * @date 2025-11-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//header guard
#ifndef _GLGE_GRAPHIC_BACKEND_API_IMPL_BUFFER_
#define _GLGE_GRAPHIC_BACKEND_API_IMPL_BUFFER_

//store the maximum amount of buffers a single buffer class may use for ring buffering
#define GLGE_BUFFER_MAX_RING_BUFFER_COUNT 4

//only available for C++
#if __cplusplus

//add all types
#include "../../../GLGE_Core/Types.h"
//add a mutex and atomics for thread safety
#include <mutex>
#include <shared_mutex>
#include <atomic>
//add a vector to queue all buffers that need re-uploading in
#include <vector>
//add intrinsics for efficient waiting
#include <immintrin.h>

//use the GLGE::Graphic::Backend::API namespace
namespace GLGE::Graphic::Backend::API
{

/**
 * @brief define the unified API for a general purpose buffer
 */
class Buffer
{
public:

    /**
     * @brief define the type of the buffer
     */
    enum Type : uint8_t
    {
        //the buffer is a shader storage buffer
        SHADER_STORAGE_BUFFER = 0,
        //the buffer is a uniform buffer
        UNIFORM_BUFFER,
        //the buffer is a vertex buffer
        VERTEX_BUFFER,
        //the buffer is an index buffer
        INDEX_BUFFER
    };

    /**
     * @brief Construct a new Buffer
     * 
     * @param data the starting data for the buffer
     * @param dataSize the starting size for the buffer
     * @param type the type of buffer to create
     */
    Buffer(void* data, uint64_t dataSize, Type type) noexcept;

    /**
     * @brief Destroy the Buffer
     */
    virtual ~Buffer() noexcept;

    /**
     * @brief store the new data in the buffer
     * 
     * @param data a pointer to the new data
     * @param dataSize the size of the data
     */
    virtual void set(void* data, uint64_t dataSize) noexcept = 0;

    /**
     * @brief write some data to a region of the buffer
     * 
     * @param data a pointer to the data to write
     * @param dataSize the size of the data to write
     * @param offset the offset into the buffer to write to
     */
    virtual void write(void* data, uint64_t dataSize, uint64_t offset) noexcept = 0;

    /**
     * @brief change the size of the buffer to a new size
     * 
     * @warning this may truncate the stored data
     * 
     * @param newSize the new size for the buffer
     */
    void resize(uint64_t newSize) noexcept;

    /**
     * @brief add some data to the currently stored data
     * 
     * @param data the data to add to the currently stored data
     * @param dataSize the size of the data to append
     */
    void append(void* data, uint64_t dataSize) noexcept;

    /**
     * @brief Get the Type of the buffer
     * 
     * @return const Type& a constant reference to the type of the buffer
     */
    inline const Type& getType() const noexcept {return m_type;}

    /**
     * @brief Get the Raw data of the buffer
     * 
     * @return const void* a pointer to the raw data
     */
    inline void* getRaw() const noexcept {return m_data;}

    /**
     * @brief Get the Size of the buffer's data
     * 
     * @return uint64_t the size of the buffered data
     */
    inline uint64_t getSize() const noexcept {return m_size;}

protected:

    /**
     * @brief update the GPU side of the buffer
     */
    virtual void update() noexcept = 0;

    /**
     * @brief queue this buffer for an update
     */
    void queueUpdate() noexcept;

    //add the instance class as a friend
    friend class Instance;

    //store the type of the buffer
    Type m_type;
    //store the current CPU side data
    void* m_data = nullptr;
    //store the size in bytes of the data
    uint64_t m_size = 0;
    //store a mutex to protect the own data
    std::shared_mutex m_dataMtx;
    //store if the buffer is queued for update
    std::atomic_bool m_queued{false};

    //store a list of all queued buffers
    inline static std::vector<Buffer*> m_queue;
    //store a mutex to make the buffer thread safe
    inline static std::mutex m_mutex;

};

/**
 * @brief a class that is used to switch buffers
 * 
 * This class does not own any data, but it owns the buffers that hold the data
 */
class BufferChain {
public:

    /**
     * @brief Construct a new Buffer Chain
     * 
     * @param count the amount of buffers to use
     */
    BufferChain(uint8_t count)
     : cm_bufferCount((count > GLGE_BUFFER_MAX_RING_BUFFER_COUNT) ? GLGE_BUFFER_MAX_RING_BUFFER_COUNT : count) 
    {
        //thread safety
        std::unique_lock lock(s_newDeleteMut);
        //add the element
        s_bufferChains.push_back(this);
    }

    /**
     * @brief Destroy the Buffer Chain
     */
    ~BufferChain() noexcept;

    /**
     * @brief actually initialize all the buffers correctly
     * 
     * @tparam T the type of buffer to create
     * @tparam Args a list of argument types to parse to the constructor
     * @param args the actual arguments to parse to the constructor
     */
    template <typename T, typename ...Args> 
    void create(Args&& ...args) noexcept {
        //sanity check
        static_assert(std::is_base_of_v<API::Buffer, T>, "The buffer class must inherit from API::Buffer to be a valid buffer class");

        //iterate over all buffers and create them
        for (uint8_t i = 0; i < cm_bufferCount; ++i) {
            m_buffers[i] = new T(std::forward<Args>(args)...);
        }
        //set the indices correctly
        m_curr.store(0, std::memory_order_relaxed);
        m_consume.store(0, std::memory_order_relaxed);
    }

    // CPU SIDE

    /**
     * @brief get the CPU buffer that is currently used for writing
     * 
     * @return API::Buffer* a pointer to the buffer that is being used for writing
     */
    inline API::Buffer* beginWrite() const noexcept
    {return m_buffers[m_curr.load(std::memory_order_acquire)];}

    /**
     * @brief this is called when writing the data to the buffer is done
     */
    void endWrite() noexcept;

    /**
     * @brief Get the buffer that is currently used by the CPU
     * 
     * @return API::Buffer* a pointer to the buffer that is currently used for writing
     */
    inline API::Buffer* getCurrent_CPU() const noexcept
    {return m_buffers[m_curr.load(std::memory_order_acquire)];}

    // GPU SIDE
    
    /**
     * @brief Get the Buffer that is being consumed by the GPU
     * 
     * @tparam T the type of buffer to quarry
     * @return T* a pointer to the buffer
     */
    template <typename T> 
    T* getBuffer_GPU() noexcept
    {return (T*)m_buffers[m_consume.load(std::memory_order_acquire)];}

    /**
     * @brief ready the next buffer for GPU consumption
     */
    void advanceGPU() noexcept;

    //make the buffer chain vector thread safe
    inline static std::shared_mutex s_newDeleteMut;
    //store all buffer chains
    inline static std::vector<BufferChain*> s_bufferChains;

protected:

    //store the actual amount of used buffers
    const uint8_t cm_bufferCount;
    //store the index of the buffer that is currently written to
    std::atomic_uint8_t m_curr{0xFFu};
    //store the index of the buffer that is currently being consumed by the GPU
    std::atomic_uint8_t m_consume{0xFF};
    //store a list of all the actual buffers
    Buffer* m_buffers[GLGE_BUFFER_MAX_RING_BUFFER_COUNT]{ nullptr };

};

}

#endif

#endif