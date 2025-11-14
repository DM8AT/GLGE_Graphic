/**
 * @file StructuredBuffer.h
 * @author DM8AT
 * @brief define a buffer that uses a specific struct for data
 * @version 0.1
 * @date 2025-11-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//header guard
#ifndef _GLGE_GRAPHIC_FRONTEND_STRUCTURED_BUFFER_
#define _GLGE_GRAPHIC_FRONTEND_STRUCTURED_BUFFER_

//add normal buffers
#include "Buffer.h"

//only available for C++
#if __cplusplus

/**
 * @brief define a class that holds a buffer of structures
 * 
 * @tparam T the type of structures to store
 */
template <typename T> class StructuredBuffer : public Buffer {
public:

    /**
     * @brief Construct a new Structured Buffer
     * 
     * @param elements a C array of elements to get the data from
     * @param elementCount the amount of elements in the array
     * @param type the type of buffer to create
     * @param bufferCount the amount of buffers to use to smooth out data syncing between GPU and CPU (default is 1)
     */
    StructuredBuffer(T* elements, uint64_t elementCount, BufferType type, uint8_t bufferCount = 1)
     : Buffer(elements, elementCount*sizeof(T), type, bufferCount)
    {}

    /**
     * @brief access a specific element
     * 
     * @param index the index of the element to access
     * @return const T& a reference to the value stored at the element
     */
    inline const T& get(uint64_t index) const noexcept
    {return *((const T*)getData() + index);}

    /**
     * @brief access a specific element
     * 
     * @param index the index of the element to access
     * @return const T& a reference to the value stored at the element
     */
    inline const T& operator[](uint64_t index) const noexcept {return get(index);}

    /**
     * @brief set the value of a specific element in the buffer
     * 
     * @param index the index of the element to set
     * @param element the element to set the data to
     */
    inline void set(uint64_t index, const T& element) noexcept
    {write((void*)&element, index*sizeof(T), sizeof(T));}

};

#endif

#endif