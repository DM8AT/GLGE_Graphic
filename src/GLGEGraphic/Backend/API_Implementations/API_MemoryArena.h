/**
 * @file GLGEGraphicAPI_MemoryArena.h
 * @author DM8AT
 * @brief define the layout for a GPU memory management arena
 * @version 0.1
 * @date 2025-06-10
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef _GLGE_GRAPHIC_BACKEND_API_IMPL_MEMORY_ARENA_
#define _GLGE_GRAPHIC_BACKEND_API_IMPL_MEMORY_ARENA_

//add all types
#include "../../../GLGE_Core/Types.h"

//check for C++
#if __cplusplus

//add the input/output stream
#include <iostream>
//add simple vectors
#include <vector>
//add a mutex for thread safety
#include <mutex>
//add buffer
#include "API_Buffer.h"

//use the GLGE::Graphic::Backend::API namespace
namespace GLGE::Graphic::Backend::API
{

/**
 * @brief store an continues area in VRam for arbitrary data storage
 */
class MemoryArena
{
public:

    /**
     * @brief store a pointer into the memory arena
     * 
     * A nullpointer is specified by the size being 0. 
     */
    struct GraphicPointer
    {
        /**
         * @brief the start index of the region
         */
        uint64_t startIdx = 0;
        /**
         * @brief the size of the region
         */
        uint64_t size = 0;

        /**
         * @brief check if two graphic pointer point to the same memory region
         * @warning the information if the memory arena is the same isn't recorded
         * 
         * @param ptr a constant reference to the other pointer
         * @return true : both pointers point to the same region
         * @return false : they point to different regions
         */
        inline bool operator==(const GraphicPointer& ptr) const noexcept {return (startIdx == ptr.startIdx) && (size == ptr.size);}

        /**
         * @brief print a graphic pointer into the default output
         * 
         * @param os the output stream to print to
         * @param ptr the pointer to print
         * @return std::ostream& the filled output stream
         */
        inline friend std::ostream& operator<<(std::ostream& os, const GraphicPointer& ptr) noexcept {return os << "GraphicPointer{index: " << ptr.startIdx << ", size: " << ptr.size << "}";}
    };

    /**
     * @brief Construct a new Graphic Memory Arena
     */
    MemoryArena() = default;

    /**
     * @brief Construct a new Graphic Memory Arena
     * 
     * @param size the size of the arena in bytes
     * @param allowResize specify if dynamic resizing is allowed
     * @param buffer a pointer to the buffer to store the data to
     */
    MemoryArena(uint64_t size, bool allowResize, Buffer* buffer) noexcept
     : m_size(size), m_allowResize(allowResize), m_free({{0,size}}), m_buff(buffer)
    {}

    /**
     * @brief Destroy the Graphic Memory Arena
     */
    virtual ~MemoryArena();

    /**
     * @brief allocate some amount of memory from the memory arena
     * 
     * @param size the size of the area to allocate
     * @return const GraphicPointer* a pointer to the requested area. The pointer belongs to the memory arena. 
     */
    GraphicPointer allocate(uint64_t size) noexcept;

    /**
     * @brief free a pointer allocated from a memory arena
     * 
     * @param ptr a reference to a pointer to the region
     * 
     * @return true : managed to free the pointer
     * @return false : failed to release the the pointer
     */
    bool release(GraphicPointer& ptr) noexcept;

    /**
     * @brief write new data into a section of VRam
     * 
     * @param ptr a pointer to the area to write
     * @param data the data to write
     */
    void update(const GraphicPointer& ptr, void* data) noexcept;

    /**
     * @brief change the size of the memory arena
     * 
     * @param size the new size of the memory arena in bytes
     */
    void resize(uint64_t size) noexcept;

    /**
     * @brief Get the Size of the memory arena
     * 
     * @return const uint64_t& the size of the memory arena
     */
    inline const uint64_t& getSize() const noexcept {return m_size;}

    /**
     * @brief Get the memory usage of the memory arena
     * 
     * @return const MemoryUsage& the memory usage of the memory arena
     */
    inline const Buffer::Type& getUsage() const noexcept {return m_buff->getType();}

    /**
     * @brief Change if the memory arena can be resized
     * @warning only use if you know what you're doing
     * 
     * @param resizable true : the memory arena will resize automatically | false : the arena won't resize automatically
     */
    inline void setResizable(bool resizable) noexcept {m_allowResize = resizable;}

    /**
     * @brief get if the memory arena resizes automatically
     * 
     * @return true : the memory arena will resize automatically
     * @return false : the arena won't resize automatically
     */
    inline bool isResizable()const noexcept {return m_allowResize;}

    /**
     * @brief Get the whole raw data of the memory arena
     * 
     * @return void* the whole raw data
     */
    inline void* getRaw() const noexcept {return m_buff->getRaw();}

    /**
     * @brief get the data of a specific pointer
     * 
     * @warning the region is not sanity-checked
     * 
     * @param ptr a pointer to the region to access
     * @return void* the data of the region
     */
    inline void* get(const GraphicPointer& ptr) {return (void*)(((uint8_t*)m_buff->getRaw()) + ptr.startIdx);}

    /**
     * @brief Get the Buffer of the memory arena
     * 
     * @return `API::Buffer*` a pointer to the buffer of the memory arena
     */
    inline API::Buffer* getBuffer() const noexcept {return m_buff;}

protected:

    /**
     * @brief store the complete size of the memory arena
     */
    uint64_t m_size = 0;
    /**
     * @brief store if dynamic resizing is allowed
     */
    bool m_allowResize = true;
    /**
     * @brief store a vector of free regions
     */
    std::vector<GraphicPointer> m_free;

    /**
     * @brief store a buffer that is used for the data storage
     */
    Buffer* m_buff = nullptr;
};

}

#endif //end of C++ section

#endif