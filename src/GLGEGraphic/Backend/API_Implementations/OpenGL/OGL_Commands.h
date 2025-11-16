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

//add render meshes
#include "../API_RenderMesh.h"

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

//add materials
class Material;

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

    /**
     * @brief Construct a new Command Container by moving from another one
     * 
     * @param other the container to move from
     */
    CommandContainer(CommandContainer&& other) noexcept {
        if (other.ptr) {
            //copy the storage over
            memcpy(storage, other.storage, sizeof(storage));
            ptr = (GLGE::Graphic::Backend::OGL::Command*)storage;
            //nullify the other pointer to stop deletion
            other.ptr = nullptr;
        }
    }

    //explicitly delete the copy constructor
    CommandContainer(CommandContainer&) = delete;

    //simple constructor
    CommandContainer() = default;
};

/**
 * @brief implement a custom command
 */
struct Command_Custom final : public Command
{
    /**
     * @brief Construct a new custom command
     * 
     * @param _func the function to call back to
     * @param _userData the user data to pass to the function
     */
    Command_Custom(void (*_func)(void*), void* _userData)
     : func(_func), userData(_userData)
    {}

    //store the function to call
    void (*func)(void*);
    //store the user data to pass to the function
    void* userData;

    //run the custom command
    virtual void execute() noexcept override;
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
 * @brief a command to bind a material in OpenGL
 */
struct Command_BindMaterial final : public Command
{
    /**
     * @brief Construct a new Bind Material command
     * 
     * @param mat a pointer to the material to bind
     */
    Command_BindMaterial(OGL::Material* mat)
     : material(mat)
    {}

    //store the material to bind
    OGL::Material* material;

    //run the actual bind command
    virtual void execute() noexcept override;
};

/**
 * @brief a command that is used to draw a simple mesh
 */
struct Command_DrawMesh final : public Command
{
    /**
     * @brief Construct a new Draw Mesh command
     * 
     * @param _rMesh a pointer to the render mesh to draw
     */
    Command_DrawMesh(API::RenderMesh* _rMesh)
     : rMesh(_rMesh)
    {}

    //store a pointer to the render mesh to draw
    API::RenderMesh* rMesh;

    //run the actual drawing command
    virtual void execute() noexcept override;
};

/**
 * @brief store a command that dispatches a compute shader
 */
struct Command_DispatchCompute final : public Command
{
    /**
     * @brief Construct a new Dispatch compute command
     * 
     * @param _compute a pointer to the frontend compute object to dispatch
     * @param _x the amount of instances to spawn on the x axis
     * @param _y the amount of instances to spawn on the y axis
     * @param _z the amount of instances to spawn on the z axis
     */
    Command_DispatchCompute(void* _compute, uint32_t _x, uint32_t _y, uint32_t _z)
     : compute(_compute), x(_x), y(_y), z(_z)
    {}

    //store a pointer to a frontend compute object
    //this is just a data collection, so no backend needed
    void* compute = nullptr;
    //store the amount of instances to dispatch
    uint32_t x, y, z;

    //run the actual dispatch and data binding
    virtual void execute() noexcept override;
};

/**
 * @brief store a command that runs a memory barrier
 */
struct Command_MemoryBarrier final : public Command
{
    /**
     * @brief Construct a new Memory barrier command
     */
    Command_MemoryBarrier() = default;

    //run the actual memory barrier
    virtual void execute() noexcept override;
};

}

#endif

#endif