/**
 * @file API_Texture.h
 * @author DM8AT
 * @brief define the virtual class for the texture binding
 * @version 0.1
 * @date 2025-10-30
 * 
 * @copyright Copyright (c) 2025
 * 
 */

//header guard
#ifndef _GLGE_GRAPHIC_BACKEND_API_IMPL_TEXTURE_
#define _GLGE_GRAPHIC_BACKEND_API_IMPL_TEXTURE_

//only available for C++
#if __cplusplus

//add a vector as a dynamic resizable array
#include <vector>
//add a mutex for thread safety
#include <mutex>
#include <shared_mutex>

//define the top-level texture class
class Texture;

//use the GLGE::Graphic::Backend::API namespace
namespace GLGE::Graphic::Backend::API
{

/**
 * @brief define the abstract texture API class
 */
class Texture
{
public:

    /**
     * @brief Construct a new Texture
     * 
     * @param tex the actual front-end texture
     */
    Texture(::Texture* tex) : m_texture(tex) {}

    /**
     * @brief mark that this class is dirty (new CPU data)
     */
    virtual void markDirty() noexcept = 0;

    /**
     * @brief notify the api texture for a resizing
     */
    virtual void notifySizeChange() noexcept = 0;

protected:

    /**
     * @brief a function to tick the texture from the GPU 
     * 
     * @warning this function is only allowed to be called from the main thread during the global main tick
     */
    virtual void tickGPU() noexcept = 0;
    
    /**
     * @brief add the API instance as a friend class
     */
    friend class Instance;

    //store a pointer to the owning front-end texture
    ::Texture* m_texture = nullptr;

    /**
     * @brief store a mutex to make GPU updates thread safe
     */
    inline static std::shared_mutex m_updateMutex;
    /**
     * @brief store a list of all backend textures that want to be ticked
     */
    inline static std::vector<Texture*> m_toUpdate;

};

}

#endif

#endif