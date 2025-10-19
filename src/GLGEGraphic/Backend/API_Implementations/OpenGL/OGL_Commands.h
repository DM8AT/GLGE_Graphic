/**
 * @file OGL_Commands.h
 * @author DM8AT
 * @brief define all commands for command buffers
 * @version 0.1
 * @date 2025-10-19
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_GRAPHIC_BACKEND_API_IMPL_OGL_COMMANDS_
#define _GLGE_GRAPHIC_BACKEND_API_IMPL_OGL_COMMANDS_

//only available for C++
#if __cplusplus

//add a lot of the STD lib for the implementation
#include <iostream>
#include <vector>
#include <type_traits>
#include <cstdint>
#include <cassert>
#include <cstring>

//add windows
class Window;

//use the namespace GLGE::Graphic::Backend::OGL
namespace GLGE::Graphic::Backend::OGL
{

/**
 * @brief define what an OpenGL command is
 */
struct Command
{
    /**
     * @brief Destroy the Command
     */
    virtual ~Command() noexcept = default;

    /**
     * @brief run the actual OpenGL command
     */
    virtual void execute() noexcept = 0;

//define some limits for a command

/**
 * @brief define the maximum amount of bytes available for parameter storage
 */
static constexpr size_t MAX_COMMAND_SIZE = 64;
/**
 * @brief define what the maximum alignment is
 */
static constexpr size_t MAX_COMMAND_ALIGN = alignof(std::max_align_t);
};

/**
 * @brief define what an actual command is
 * 
 * Just a base class for all commands to implement
 */
struct CommandContainer
{
    /**
     * @brief store the command data
     */
    alignas(Command::MAX_COMMAND_ALIGN) std::byte storage[Command::MAX_COMMAND_SIZE];
    /**
     * @brief encapsulate a base command (do not inherit from it)
     * 
     * This should point to the start of storage only if the element exists
     */
    Command* ptr = nullptr;

    /**
     * @brief make this an actual command
     * 
     * @tparam T the type of command to create
     * @tparam Args the type of arguments to create
     * @param args the actual templates
     */
    template <typename T, typename ...Args>
    void emplace(Args&& ...args) {
        //sanity check the inputs (static to do at compile time)
        static_assert(sizeof(T) <= Command::MAX_COMMAND_SIZE, "CommandContainer too large");
        static_assert(std::is_base_of<Command, T>().value, "T must derive from command");
        //create the actual commands
        ptr = new (storage) T(std::forward<Args>(args)...);
    }

    /**
     * @brief run the actual command
     */
    inline void execute() noexcept {ptr->execute();}

    /**
     * @brief clean up the command
     */
    void destroy() noexcept {
        if (ptr) {
            ptr->~Command();
            ptr = nullptr;
        }
    }

    /**
     * @brief Destroy the CommandContainer
     */
    ~CommandContainer() {destroy();}

    //simple constructor
    CommandContainer() = default;
};

/**
 * @brief clear command for OpenGL implementation
 */
struct Command_Clear final : public Command
{
    /**
     * @brief Construct a new clear command
     * 
     * @param _r the red part of the color to clear with
     * @param _g the green part of the color to clear with
     * @param _b the blue part of the color to clear with
     * @param _a the alpha part of the color to clear with
     * @param _fbuff the name of the framebuffer to clear
     * @param _buffType the type of the buffer attachment to clear
     * @param _buffId the ID of the buffer to clear
     */
    Command_Clear(float _r, float _g, float _b, float _a, uint32_t _fbuff, uint32_t _buffType, uint32_t _buffId) noexcept
     : r(_r), g(_g), b(_b), a(_a), fbuff(_fbuff), buffType(_buffType), buffId(_buffId)
    {}

    //store the color to clear
    float r = 0, g = 0, b = 0, a = 1;
    //store the framebuffer object to clear
    uint32_t fbuff = 0;
    //store the type of buffer to clear
    uint32_t buffType = 0;
    uint32_t buffId = 0;

    //run the actual OpenGL command
    virtual void execute() noexcept override;
};

/**
 * @brief store a command to swap a window
 */
struct Command_Swap final : public Command
{
    /**
     * @brief Construct a new swap command
     * 
     * @param _window a pointer to the window to swap
     */
    Command_Swap(::Window* _window)
     : window(_window)
    {}

    //store the window to swap
    ::Window* window;

    //swap the stored window
    virtual void execute() noexcept override;
};

/**
 * @brief make a window the current window to work on
 */
struct Command_MakeCurrent final : public Command
{
    /**
     * @brief Construct a new make current command
     * 
     * @param _window a pointer to the window to make current
     */
    Command_MakeCurrent(::Window* _window)
     : window(_window)
    {}

    //store the window to make the current window
    ::Window* window;

    //make this the current window
    virtual void execute() noexcept override;
};

}

#endif

#endif