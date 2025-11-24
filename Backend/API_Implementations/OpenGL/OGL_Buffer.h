/**
 * @file OGL_Buffer.h
 * @author DM8AT
 * @brief overload the unified API for an OpenGL buffer
 * @version 0.1
 * @date 2025-11-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//header guard
#ifndef _GLGE_GRAPHIC_BACKEND_API_IMPL_OGL_BUFFER_
#define _GLGE_GRAPHIC_BACKEND_API_IMPL_OGL_BUFFER_

//only available for C++
#if __cplusplus

//add the API
#include "../API_Buffer.h"

//use the namespace GLGE::Graphic::Backend::OGL
namespace GLGE::Graphic::Backend::OGL
{

/**
 * @brief define the overload for the OpenGL buffer
 */
class Buffer final : public API::Buffer
{
public:

    /**
     * @brief Construct a new Buffer
     * 
     * @param data the initial data for the buffer
     * @param dataSize the size of the data
     * @param type the type of the buffer
     */
    Buffer(void* data, uint64_t dataSize, Type type) noexcept : API::Buffer(data, dataSize, type) {}

    /**
     * @brief Destroy the Buffer
     */
    virtual ~Buffer();

    /**
     * @brief store the new data in the buffer
     * 
     * @param data a pointer to the new data
     * @param dataSize the size of the data
     */
    virtual void set(void* data, uint64_t dataSize) noexcept override;

    /**
     * @brief write some data to a region of the buffer
     * 
     * @param data a pointer to the data to write
     * @param dataSize the size of the data to write
     * @param offset the offset into the buffer to write to
     */
    virtual void write(void* data, uint64_t dataSize, uint64_t offset) noexcept override;

    /**
     * @brief Get the Buffer
     * 
     * @return uint32_t the actual OpenGL buffer
     */
    inline uint32_t getBuffer() const noexcept {return m_buff;}

protected:

    /**
     * @brief add the instance as a friend class
     */
    friend class Instance;

    /**
     * @brief force the buffer to create itself, even if it is empty
     */
    void forceCreate();

    /**
     * @brief update this GPU buffer
     */
    virtual void update() noexcept override;

    //store the OpenGL buffer
    uint32_t m_buff = 0;
    //store the current buffer size
    uint64_t m_currSize = 0;
    //store a pointer to the mapped data
    void* m_mappedPtr = nullptr;

};

}

#endif

#endif