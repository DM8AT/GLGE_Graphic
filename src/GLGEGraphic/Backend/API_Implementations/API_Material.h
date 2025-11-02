/**
 * @file API_Material.h
 * @author DM8AT
 * @brief define the unified API for materials
 * @version 0.1
 * @date 2025-11-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */
//header guard
#ifndef _GLGE_GRAPHIC_BACKEND_API_IMPL_MATERIL_
#define _GLGE_GRAPHIC_BACKEND_API_IMPL_MATERIL_

//only available for C++
#if __cplusplus

//add the command buffer
#include "API_CommandBuffer.h"

//material class will be implemented later
class Material;

//use the GLGE::Graphic::Backend::API namespace
namespace GLGE::Graphic::Backend::API
{

/**
 * @brief store the abstract base class for the material backend
 */
class Material
{
public:

    /**
     * @brief Construct a new Material
     * 
     * @param material a pointer to the frontend material
     */
    Material(::Material* material) noexcept : m_material(material) {}

    /**
     * @brief Destroy the Material
     */
    virtual ~Material() = default;

    /**
     * @brief bind the material as the currently active one
     * 
     * @param cmdBuff a pointer to the command buffer to queue the command into
     */
    virtual void bind(API::CommandBuffer* cmdBuff) noexcept = 0;

    /**
     * @brief Get the frontend Material
     * 
     * @return ::Material* a pointer to the frontend material
     */
    inline ::Material* getMaterial() noexcept {return m_material;}

protected:

    //store a pointer to the frontend material
    ::Material* m_material = nullptr;

};

}

#endif

#endif