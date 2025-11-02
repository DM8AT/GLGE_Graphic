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
    void set(void* data, uint64_t dataSize) noexcept;

    /**
     * @brief add some data to the currently stored data
     * 
     * @param data the data to add to the currently stored data
     * @param dataSize the size of the data to append
     */
    void append(void* data, uint64_t dataSize) noexcept;

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

}

#endif

#endif