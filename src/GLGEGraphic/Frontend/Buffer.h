/**
 * @file Buffer.h
 * @author DM8AT
 * @brief define the interface for GPU transfer buffers in GLGE
 * @version 0.1
 * @date 2025-11-11
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//header guard
#ifndef _GLGE_GRAPHIC_FRONTEND_BUFFER_
#define _GLGE_GRAPHIC_FRONTEND_BUFFER_

//add types
#include "../../GLGE_Core/Types.h"

//define the type of buffer to use
typedef enum e_BufferType {
    //the buffer is small, but fast to access
    //the buffer is read only
    GLGE_BUFFER_TYPE_UNIFORM = 0,
    //the buffer is larger, but access is slower
    //the buffer can be written and read from
    GLGE_BUFFER_TYPE_SHADER_STORAGE
} BufferType;

//define the class for C++
#if __cplusplus

/**
 * @brief store a raw data area that will also live on the GPU
 */
class Buffer 
{
public:

    /**
     * @brief Construct a new Buffer
     */
    Buffer() = default;

    /**
     * @brief Construct a new Buffer
     * 
     * @param data the raw data to fill the buffer with
     * @param size the size of the buffer
     * @param type the type of the buffer
     * @param bufferCount the amount of buffers to use to smooth out data syncing between GPU and CPU (default is 1)
     */
    Buffer(void* data, uint64_t size, BufferType type, uint8_t bufferCount = 1);

    /**
     * @brief Get the Type of the buffer
     * 
     * @return BufferType the type of the buffer
     */
    inline BufferType getType() const noexcept {return m_type;}

    /**
     * @brief Get the Backend data
     * 
     * @warning only use if you know what you're doing
     * 
     * @return void* a pointer to the backend object
     */
    inline void* getBackend() const noexcept {return m_buff;}

    /**
     * @brief Get the Data stored in the buffer
     * 
     * @return const void* a pointer to the raw data
     */
    const void* getData() const noexcept;

    /**
     * @brief Get the Size of the buffer in bytes
     * 
     * @return uint64_t the size of the buffer in bytes
     */
    uint64_t getSize() const noexcept;

    /**
     * @brief Set the Data for the whole buffer
     * 
     * @param data the data to write
     * @param size the size of the new data
     */
    void setData(void* data, uint64_t size) noexcept;

    /**
     * @brief write data to a region of the buffer
     * 
     * @param data the data to write to the region
     * @param offset the offset into the buffer to write to
     * @param size the size of the data in bytes to write
     */
    void write(void* data, uint64_t offset, uint64_t size) noexcept;

    /**
     * @brief change the size of the buffer
     * 
     * @warning this may truncate the stored data
     * 
     * @param size the new size of the buffer
     */
    void resize(uint64_t size) noexcept;

protected:

    //store the type of the buffer
    BufferType m_type = GLGE_BUFFER_TYPE_SHADER_STORAGE;
    //store the backend buffer
    uint8_t m_buffStorage[96];
    //store a pointer to the buffer (null means uninitialized)
    void* m_buff = nullptr;

};

#else

//define an opaque structure
typedef struct s_Buffer {byte opaque;} Buffer;

#endif

//define a value that means that this is an unused shader storage buffer
#define GLGE_SKIP_SLOT_SHADER_STORAGE_BUFFER ((Buffer*)0xFF01)
//define a value that means that this is an unused uniform buffer
#define GLGE_SKIP_SLOT_UNIFORM_BUFFER ((Buffer*)0xFF02)

#endif