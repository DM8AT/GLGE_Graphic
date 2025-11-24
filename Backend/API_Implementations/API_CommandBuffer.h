/**
 * @file API_CommandBuffer.h
 * @author DM8AT
 * @brief define an abstract class for command buffers
 * @version 0.1
 * @date 2025-10-18
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_GRAPHICS_BACKEND_API_IMPL_API_COMMAND_BUFFER_
#define _GLGE_GRAPHICS_BACKEND_API_IMPL_API_COMMAND_BUFFER_

//only available for C++
#if __cplusplus

//use the GLGE::Graphic::Backend::API namespace
namespace GLGE::Graphic::Backend::API
{

/**
 * @brief define what an abstract command buffer is
 */
class CommandBuffer
{
public:

    /**
     * @brief Construct a new Command Buffer
     */
    CommandBuffer() = default;

    /**
     * @brief Destroy the Command Buffer
     */
    virtual ~CommandBuffer() {}

    /**
     * @brief get if the command buffer is recorded
     * 
     * @return true : the command buffer is recorded
     * @return false : the command buffer is not recorded
     */
    inline bool isRecorded() const noexcept {return m_recorded;}

    /**
     * @brief play back the command buffer. Does nothing if the command buffer is not recorded. 
     */
    virtual void play() noexcept = 0;

    /**
     * @brief clear the recorded contents
     */
    virtual void clear() noexcept = 0;
    
protected:

    /**
     * @brief store if the command buffer is currently recorded
     */
    bool m_recorded = false;

};

}

#endif

#endif