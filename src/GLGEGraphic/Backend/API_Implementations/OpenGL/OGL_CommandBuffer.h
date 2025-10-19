/**
 * @file OGL_CommandBuffer.h
 * @author DM8AT
 * @brief define what a command buffer for OpenGL is
 * @version 0.1
 * @date 2025-10-18
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_GRAPHIC_BACKEND_API_IMPL_OPEN_GL_COMMAND_BUFFER_
#define _GLGE_GRAPHIC_BACKEND_API_IMPL_OPEN_GL_COMMAND_BUFFER_

//add the command buffer API
#include "../API_CommandBuffer.h"
//add all types
#include "../../../../GLGE_Core/Types.h"
//add debugging
#include "../../../../GLGE_BG/Debugging/Logging/__BG_SimpleDebug.h"
//add the commands
#include "OGL_Commands.h"

//only available for C++
#if __cplusplus

//use the namespace GLGE::Graphic::Backend::OGL
namespace GLGE::Graphic::Backend::OGL
{

/**
 * @brief a command buffer just contains a list of commands to execute
 */
class CommandBuffer final : public GLGE::Graphic::Backend::API::CommandBuffer
{
public:

    /**
     * @brief Construct a new CommandContainer Buffer
     */
    CommandBuffer() = default;

    /**
     * @brief Destroy the CommandContainer Buffer
     */
    virtual ~CommandBuffer() {}

    /**
     * @brief clear all recorded contents
     */
    virtual void clear() noexcept override;

    /**
     * @brief play back the recorded commands
     */
    virtual void play() noexcept override;

    /**
     * @brief record a new command
     * 
     * @tparam T the type of command to record
     * @tparam Args the arguments to pass to the command
     * @param args the actual arguments to pass
     */
    template <typename T, typename ...Args>
    void record(Args&& ...args) {
        //sanity check if the state is correct
        GLGE_DEBUG_ASSERT("Adding a new command to a command buffer that is marked as recorded", m_recorded);
        //add the command to the back of the command buffer
        m_commands.emplace_back();
        m_commands.back().template emplace<T>(std::forward<Args>(args) ...);
    }

    /**
     * @brief mark this command buffer as recorded
     */
    inline void markRecorded() noexcept {m_recorded = true;}

protected:

    /**
     * @brief store all commands
     */
    std::vector<CommandContainer> m_commands;

};

}

#endif

#endif